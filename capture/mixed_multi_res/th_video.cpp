#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/syscall.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>
#include <sys/mman.h>
#include <unistd.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <chrono>
#include <opencv2/objdetect.hpp>
#include <iostream>
#define CV_NO_BACKWARD_COMPATIBILITY
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#define switch_stream
#include <thread>
//#define no_process
using namespace std;
using namespace cv;
int fd;
int scaler=1;
int regions = 3;
vector<uchar> buf;  
int resolution_region[4]= {0,0,0,0};
int type;
void* buffer_start;
int delay;
int height=950;//1080;
int width=950;//1080;
Size S;
const string filename="/run/user/1000/images/test.mp4";
int fourcc = VideoWriter::fourcc('M','J','P','G');
//int fourcc = VideoWriter::fourcc('M','P','E','G');
double fps= 4;
VideoWriter writer;
VideoWriter writer2;
VideoWriter writer3;
VideoWriter writer4;
IplImage *src,*dst,*src1,*src3;
Mat wFrame;
struct v4l2_control control;
struct v4l2_format formats;
struct v4l2_capability cap;
struct v4l2_requestbuffers bufrequest;
struct v4l2_buffer bufferinfo;
int resx[15] = { width, 1100, 900, 900, 800, 700,  600, 600, 512 , 440, 360, 224, 160, 96, 70};
int resy[15] = { height,  800,  900, 800, 800, 800 , 800, 600, 512 , 440, 360, 224, 160, 96, 70};
int shared_region=0;
int scale[4]= {1,1,1,1}; 
int iter;
CvSize sz = { width, height };
cv::Rect roi;
IplImage *bayer, *rgb;
//int copied = 1;
int frame_ready= 0;
int done=0;
int global_delay=125000 ;
//for light weight face detection
cv::CascadeClassifier classifier("face.xml");
std::vector<cv::Rect> features;
void capture()
{
    int val = 0;
    while(1)
    {
        auto t1 = std::chrono::high_resolution_clock::now();
        if(1)
        {
//            printf("capturing %0d\n",val);
            //printf("Acquiring frame id %0d\n",val+1);
    
            //printf("Acquiring frame id %0d\n",val+1);
    #ifdef switch_stream
        	if(ioctl(fd, VIDIOC_STREAMON, &type) < 0){
        	    perror("VIDIOC_STREAMON");
        	    exit(1);
        	}
    #endif
            // The buffer's waiting in the outgoing queue.
            if(ioctl(fd, VIDIOC_QBUF, &bufferinfo) < 0){
                perror("VIDIOC_QBUF");
                exit(1);
            }
            if(ioctl(fd, VIDIOC_DQBUF, &bufferinfo) < 0){
                perror("VIDIOC_QBUF");
                exit(1);
        	}
    #ifdef switch_stream
            if(ioctl(fd, VIDIOC_STREAMOFF, &type) < 0){ perror("VIDIOC_STREAMOFF");
               exit(1);
            }
    #endif
        //copied=0;
        frame_ready=1;
        val+=1;
     }
     auto t2 = std::chrono::high_resolution_clock::now();
     auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
     delay = global_delay- int(duration);
    std::cout<<"capture "<<delay<<std::endl;
    if(delay<0){
        delay=0;
    }
    // std::cout << delay<<std::endl;
     usleep(delay);
     if(done){
         break;
     }
   }
}
void sync()
{
    printf("synching..\n");
    while(frame_ready==0){
        usleep(1000);
    }
    if(delay<0)
        delay =0;
    usleep(delay);
}
void process()
{
    int val = 0;
    int delay2=0;
    int synch=0;
    sync();
    while(val<iter) 
    {
        if(synch){
            synch=0;
            sync();
        }
        auto t1 = std::chrono::high_resolution_clock::now();
        if(frame_ready)
        {
        #ifndef no_process
           roi.x =0; //1200     // 950
           roi.y =0; //350      //150 
           roi.width = resx[0];  //2360          //2750
           roi.height = resy[0];  //1235 /2
            printf("processing %0d\n",val);
	    	//bayer = cvCreateImage({1920,1080}, IPL_DEPTH_8U, 1);
	    	//bayer->imageData = (char *)(buffer_start);
	    	//cvSetImageROI(bayer, roi);
            src= cvLoadImage("face.jpeg",CV_LOAD_IMAGE_GRAYSCALE);
	    	//src = cvCreateImage(sz, IPL_DEPTH_8U, 1);
            //cvCopy(bayer, src);
           // auto t3 = std::chrono::high_resolution_clock::now();
 	       // auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>( t3 - t1 ).count();
           // std::cout << duration1<<std::endl;
            frame_ready=0;
            wFrame= cvarrToMat(src);
            classifier.detectMultiScale(wFrame, features, 1.1, 2,0 | CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));
            for (auto&& rect : features) {
                            cvRectangle(src,                  
                                  cvPoint(rect.x, rect.y),    
                                  cvPoint(rect.x+rect.width, rect.y+rect.height),
                                  cvScalar(255, 255, 255, 0),
                                  2, 8, 0);
                }
            if(regions ==1){
                roi.x =0; //1200     // 950
                roi.y =0; //350      //150 
                roi.width = resx[0];  //2360          //2750
                roi.height = resy[0];  //1235 /2
	    	    cvSetImageROI(src, roi);
	    	    dst = cvCreateImage({roi.width,roi.height}, IPL_DEPTH_8U, 3);
	    	    cvCvtColor(src, dst, CV_BayerRG2BGR);
	    	    cvReleaseImage(&src);
	    	    cvReleaseImage(&bayer);
                wFrame= cvarrToMat(dst);
                writer.write(wFrame);
	    	    cvReleaseImage(&dst);
            }
            else if (regions==2){
                roi.x =0; //1200    // 950
                roi.y =0; //350     //150 
                roi.width = int(resx[0]/2);  //2360          //2750
                roi.height = resy[0];  //1235 
	    	    cvSetImageROI(src, roi);
	    	    src3 = cvCreateImage(cvSize(roi.width,roi.height), IPL_DEPTH_8U, 1);
                cvCopy(src, src3);
	    	    src1 = cvCreateImage(cvSize(roi.width/scale[0],roi.height/scale[0]), IPL_DEPTH_8U, 1);
                cvResize(src3,src1,CV_INTER_LINEAR);
	    	    dst = cvCreateImage({roi.width/scale[0],roi.height/scale[0]}, IPL_DEPTH_8U, 3);
	    	    cvCvtColor(src1, dst, CV_BayerRG2BGR);
	    	    cvReleaseImage(&src1);
	    	    cvReleaseImage(&bayer);
                wFrame= cvarrToMat(dst);
                writer.write(wFrame); 
	    	    cvReleaseImage(&dst);
///////////////////////////////////////////////////////
                roi.x =roi.width; //1200    // 950
                roi.y =0; //350     //150 
                roi.width = int(resx[0]/2);  //2360          //2750
                roi.height = resy[0];  //1235 
	    	    cvSetImageROI(src, roi);
	    	    src3 = cvCreateImage(cvSize(roi.width,roi.height), IPL_DEPTH_8U, 1);
                cvCopy(src, src3);
	    	    src1 = cvCreateImage(cvSize(int(roi.width/scale[1]),int(roi.height/scale[1])), IPL_DEPTH_8U, 1);
                cvResize(src3,src1,CV_INTER_LINEAR);
	    	    dst = cvCreateImage({int(roi.width/scale[1]),int(roi.height/scale[1])}, IPL_DEPTH_8U, 3);
	    	    cvCvtColor(src1, dst, CV_BayerRG2BGR);
	    	    cvReleaseImage(&src1);
	    	    cvReleaseImage(&bayer);
                wFrame= cvarrToMat(dst);
                writer2.write(wFrame); 
	    	    cvReleaseImage(&dst);
            }
            else if (regions==3){
                roi.x =0; //1200    // 950
                roi.y =0; //350     //150 
                roi.width = int(resx[0]/2);  //2360          //2750
                roi.height = resy[0];  //1235 
	    	    cvSetImageROI(src, roi);
	    	    src3 = cvCreateImage(cvSize(roi.width,roi.height), IPL_DEPTH_8U, 1);
                cvCopy(src, src3);
	    	    src1 = cvCreateImage(cvSize(roi.width/scale[0],roi.height/scale[0]), IPL_DEPTH_8U, 1);
                cvResize(src3,src1,CV_INTER_LINEAR);
	    	    dst = cvCreateImage({roi.width/scale[0],roi.height/scale[0]}, IPL_DEPTH_8U, 3);
	    	    cvCvtColor(src1, dst, CV_BayerRG2BGR);
	    	    cvReleaseImage(&src1);
	    	    cvReleaseImage(&bayer);
                wFrame= cvarrToMat(dst);
                writer.write(wFrame); 
	    	    cvReleaseImage(&dst);
///////////////////////////////////////////////////////
                roi.x =resx[0]/2; //1200    // 950
                roi.y =0; //350     //150 
                roi.width = int(resx[0]/2);  //2360          //2750
                roi.height = resy[0]/2;  //1235 
	    	    cvSetImageROI(src, roi);
	    	    src3 = cvCreateImage(cvSize(roi.width,roi.height), IPL_DEPTH_8U, 1);
                cvCopy(src, src3);
	    	    src1 = cvCreateImage(cvSize(roi.width/scale[1],roi.height/scale[1]), IPL_DEPTH_8U, 1);
                cvResize(src3,src1,CV_INTER_LINEAR);
	    	    dst = cvCreateImage({roi.width/scale[1],roi.height/scale[1]}, IPL_DEPTH_8U, 3);
	    	    cvCvtColor(src1, dst, CV_BayerRG2BGR);
	    	    cvReleaseImage(&src1);
	    	    cvReleaseImage(&bayer);
                wFrame= cvarrToMat(dst);
                writer2.write(wFrame); 
	    	    cvReleaseImage(&dst);
///////////////////////////////////////////////////////
                roi.x =resx[0]/2; //1200    // 950
                roi.y =resy[0]/2; //350     //150 
                roi.width = int(resx[0]/2);  //2360          //2750
                roi.height = int(resy[0]/2);  //1235 
	    	    cvSetImageROI(src, roi);
	    	    src3 = cvCreateImage(cvSize(roi.width,roi.height), IPL_DEPTH_8U, 1);
                cvCopy(src, src3);
	    	    src1 = cvCreateImage(cvSize(roi.width/scale[2],roi.height/scale[2]), IPL_DEPTH_8U, 1);
                cvResize(src3,src1,CV_INTER_LINEAR);
	    	    dst = cvCreateImage({roi.width/scale[2],roi.height/scale[2]}, IPL_DEPTH_8U, 3);
	    	    cvCvtColor(src1, dst, CV_BayerRG2BGR);
	    	    cvReleaseImage(&src1);
	    	    cvReleaseImage(&bayer);
                wFrame= cvarrToMat(dst);
                writer3.write(wFrame); 
	    	    cvReleaseImage(&dst);
            }
            else if (regions==4){
                roi.x =0; //1200    // 950
                roi.y =0; //350     //150 
                roi.width = int(resx[0]/2);  //2360          //2750
                roi.height = resy[0]/2;  //1235 
	    	    cvSetImageROI(src, roi);
	    	    src3 = cvCreateImage(cvSize(roi.width,roi.height), IPL_DEPTH_8U, 1);
                cvCopy(src, src3);
	    	    src1 = cvCreateImage(cvSize(roi.width/scale[0],roi.height/scale[0]), IPL_DEPTH_8U, 1);
                cvResize(src3,src1,CV_INTER_LINEAR);
	    	    dst = cvCreateImage({roi.width/scale[0],roi.height/scale[0]}, IPL_DEPTH_8U, 3);
	    	    cvCvtColor(src1, dst, CV_BayerRG2BGR);
	    	    cvReleaseImage(&src1);
	    	    cvReleaseImage(&bayer);
                wFrame= cvarrToMat(dst);
                writer.write(wFrame); 
	    	    cvReleaseImage(&dst);
///////////////////////////////////////////////////////
                roi.x =0; //1200    // 950
                roi.y =resy[0]/2; //350     //150 
                roi.width = int(resx[0]/2);  //2360          //2750
                roi.height = resy[0]/2;  //1235 
	    	    cvSetImageROI(src, roi);
	    	    src3 = cvCreateImage(cvSize(roi.width,roi.height), IPL_DEPTH_8U, 1);
                cvCopy(src, src3);
	    	    src1 = cvCreateImage(cvSize(roi.width/scale[1],roi.height/scale[1]), IPL_DEPTH_8U, 1);
                cvResize(src3,src1,CV_INTER_LINEAR);
	    	    dst = cvCreateImage({roi.width/scale[1],roi.height/scale[1]}, IPL_DEPTH_8U, 3);
	    	    cvCvtColor(src1, dst, CV_BayerRG2BGR);
	    	    cvReleaseImage(&src1);
	    	    cvReleaseImage(&bayer);
                wFrame= cvarrToMat(dst);
                writer2.write(wFrame); 
	    	    cvReleaseImage(&dst);
///////////////////////////////////////////////////////
                roi.x =resx[0]/2; //1200    // 950
                roi.y =0; //350     //150 
                roi.width = int(resx[0]/2);  //2360          //2750
                roi.height = resy[0]/2;  //1235 
	    	    cvSetImageROI(src, roi);
	    	    src3 = cvCreateImage(cvSize(roi.width,roi.height), IPL_DEPTH_8U, 1);
                cvCopy(src, src3);
	    	    src1 = cvCreateImage(cvSize(roi.width/scale[2],roi.height/scale[2]), IPL_DEPTH_8U, 1);
                cvResize(src3,src1,CV_INTER_LINEAR);
	    	    dst = cvCreateImage({roi.width/scale[2],roi.height/scale[2]}, IPL_DEPTH_8U, 3);
	    	    cvCvtColor(src1, dst, CV_BayerRG2BGR);
	    	    cvReleaseImage(&src1);
	    	    cvReleaseImage(&bayer);
                wFrame= cvarrToMat(dst);
                writer3.write(wFrame); 
	    	    cvReleaseImage(&dst);
///////////////////////////////////////////////////////
                roi.x =resx[0]/2; //1200    // 950
                roi.y =resy[0]/2; //350     //150 
                roi.width = int(resx[0]/2);  //2360          //2750
                roi.height = int(resy[0]/2);  //1235 
	    	    cvSetImageROI(src, roi);
	    	    src3 = cvCreateImage(cvSize(roi.width,roi.height), IPL_DEPTH_8U, 1);
                cvCopy(src, src3);
	    	    src1 = cvCreateImage(cvSize(roi.width/scale[3],roi.height/scale[3]), IPL_DEPTH_8U, 1);
                cvResize(src3,src1,CV_INTER_LINEAR);
	    	    dst = cvCreateImage({roi.width/scale[3],roi.height/scale[3]}, IPL_DEPTH_8U, 3);
	    	    cvCvtColor(src1, dst, CV_BayerRG2BGR);
	    	    cvReleaseImage(&src1);
	    	    cvReleaseImage(&bayer);
                wFrame= cvarrToMat(dst);
                writer4.write(wFrame); 
	    	    cvReleaseImage(&dst);
            }
            #else 
                frame_ready=0;
            #endif
            val+=1;
        }
        auto t2 = std::chrono::high_resolution_clock::now();
 	    auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
        delay2 = global_delay- int(duration);
        std::cout << delay2<<std::endl;
        if(delay2<0){
            delay2=0;
            synch=1;
            frame_ready=0;
        }
//        global_delay  = 150000;
        usleep(delay2);
        
    }
    done=1;
}
void setup()
{
    if((fd = open("/dev/video0", O_RDWR)) < 0){
        perror("open");
        exit(1);
    }
    printf("successfully openned the device\n");
    if(ioctl(fd, VIDIOC_QUERYCAP, &cap) < 0){
        perror("VIDIOC_QUERYCAP");
        exit(1);
    }
    if(!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)){
        fprintf(stderr, "The device does not handle single-planar video capture.\n");
        exit(1);
    }
    printf("capablity check passed\n");
    formats.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    formats.fmt.pix.pixelformat = V4L2_PIX_FMT_SRGGB8;
    formats.fmt.pix.width = 1920;
    formats.fmt.pix.height = 1080;
    if(ioctl(fd, VIDIOC_S_FMT, &formats) < 0){
        perror("VIDIOC_S_FMT");
        exit(1);
    }
    printf("set format successfull\n");
    bufrequest.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    bufrequest.memory = V4L2_MEMORY_MMAP;
    bufrequest.count = 1;
    if(ioctl(fd, VIDIOC_REQBUFS, &bufrequest) < 0){
        perror("VIDIOC_REQBUFS");
        exit(1);
    }
    memset(&bufferinfo, 0, sizeof(bufferinfo));
    bufferinfo.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    bufferinfo.memory = V4L2_MEMORY_MMAP;
    bufferinfo.index = 0;
    if(ioctl(fd, VIDIOC_QUERYBUF, &bufferinfo) < 0){
        perror("VIDIOC_QUERYBUF");
        exit(1);
    }
    bufferinfo.index = 0; /* Queueing buffer index 0. */
    buffer_start = mmap(
        NULL,
        bufferinfo.length,
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        fd,
        bufferinfo.m.offset
    );
 
    if(buffer_start == MAP_FAILED){
        perror("mmap");
        exit(1);
    }
    type = bufferinfo.type;
    if(regions==1){
        S = Size(width,height);
  //      writer.open("/run/user/1000/images/test.avi",fourcc,fps,S);
        writer.open("test.avi",fourcc,fps,S);
    }
    else if (regions==2)
    {
        S = Size(width/(2*scale[0]),height/(scale[0]));
        writer.open("test.avi",fourcc,fps,S);
        S = Size(width/(2*scale[1]),height/(scale[1]));
        writer2.open("test2.avi",fourcc,fps,S);
    }
    else if (regions==3)
    {
        S = Size(width/(2*scale[0]),height/(scale[0]));
        writer.open("test.avi",fourcc,fps,S);
        S = Size(width/(2*scale[1]),height/(2*scale[1]));
        writer2.open("test2.avi",fourcc,fps,S);
        S = Size(width/(2*scale[2]),height/(2*scale[2]));
        writer3.open("test3.avi",fourcc,fps,S);
    }
    else if (regions==4)
    {
        S = Size(width/(2*scale[0]),height/(2*scale[0]));
        writer.open("test.avi",fourcc,fps,S);
        S = Size(width/(2*scale[1]),height/(2*scale[1]));
        writer2.open("test2.avi",fourcc,fps,S);
        S = Size(width/(2*scale[2]),height/(2*scale[2]));
        writer3.open("test3.avi",fourcc,fps,S);
        S = Size(width/(2*scale[3]),height/(2*scale[3]));
        writer4.open("test4.avi",fourcc,fps,S);
    }
}
int main(int argc, char** argv){
    regions=atoi(argv[1]);
    iter=atoi(argv[2]);
    for (int ii =0; ii<regions; ii++){
        scale[ii]= atoi(argv[3+ii]);
    }
    system("echo r> running.re");
    setup();
    // Activate streaming
    #ifndef switch_stream
        	if(ioctl(fd, VIDIOC_STREAMON, &type) < 0){
        	    perror("VIDIOC_STREAMON");
        	    exit(1);
        	}
    #endif
        //printf("Acquiring frame id %0d\n",val+1);
	    thread th1(capture);
	    thread th2(process);
        th1.join();
        th2.join();
        //capture();
        //process();
    #ifndef switch_stream
        	if(ioctl(fd, VIDIOC_STREAMOFF, &type) < 0){
        	    perror("VIDIOC_STREAMON");
        	    exit(1);
        	}
    #endif
    system("rm running.re");
    writer.release();
    writer2.release();
    
    close(fd);
    return EXIT_SUCCESS;
}

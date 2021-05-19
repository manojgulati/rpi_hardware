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
#include <iostream>
#define CV_NO_BACKWARD_COMPATIBILITY
//#define jpeg
//#include <cv.h>
//#include <highgui.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#define switch_stream
using namespace std;
using namespace cv;
int main(int argc, char** argv){
    int fd;
    int val = 0;
	int scaler=1;
    vector<uchar> buf;  
	int resolution_region[4]= {0,0,0,0};
	int type;
    int x1,x2,y1,y2;
    int p[3];
    float delay;
    int height=1080;
    int width=1920;
    Size S;
    const string filename="/run/user/1000/images/test.avi";
    int fourcc = VideoWriter::fourcc('M','J','P','G');
    //int fourcc = VideoWriter::fourcc('D','I','V','X');
    double fps= 4;
    
    VideoWriter writer;
    VideoWriter writer2;
	IplImage *src,*dst,*src1,*src2,*src3,*src4;

    Mat wFrame;
    struct v4l2_control control;
    struct v4l2_format format;
    struct v4l2_capability cap;
    struct v4l2_requestbuffers bufrequest;
    struct v4l2_buffer bufferinfo;
	int resx[15] = { 1920, 1100, 900, 900, 800, 700,  600, 600, 512 , 440, 360, 224, 160, 96, 70};
    int resy[15] = { 1080,  800,  900, 800, 800, 800 , 800, 600, 512 , 440, 360, 224, 160, 96, 70};
	int res=0;
    int shared_region=0;
    int scale=4; 
    int iter;
	CvSize sz = { 1920, 1080 };
	cv::Rect roi;
	IplImage *bayer, *rgb;
    int jpgfile;
    char fn[32];
	char *fn1;
	fn1 = (char *)&fn;
    shared_region=atoi(argv[1]);
    iter=atoi(argv[2]);
    system("rm running.re");
    sleep(0.5);
    system("echo r > running.re");
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
    format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    format.fmt.pix.pixelformat = V4L2_PIX_FMT_SRGGB8;
    format.fmt.pix.width = 1920;
    format.fmt.pix.height = 1080;
    if(ioctl(fd, VIDIOC_S_FMT, &format) < 0){
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
    void* buffer_start = mmap(
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
#ifndef switch_stream     
    	if(ioctl(fd, VIDIOC_STREAMON, &type) < 0){
    	    perror("VIDIOC_STREAMON");
    	    exit(1);
    	}
#endif
     
    // Activate streaming
    printf("streaming\n");
    memset(&control, 0, sizeof(control));
    control.value=0;//(val%4)+10+ ((resolution_region[0])<<4);
        control.id = V4L2_CID_TEST_PATTERN_RED;
        if(ioctl(fd, VIDIOC_S_CTRL, &control) < 0){
            perror("VDIOC_S_CTRL");
            exit(1);
        }
    
    if(shared_region==0){
        S = Size(width,height);
        //writer.open("/run/user/1000/images/test.avi",fourcc,fps,S);
        writer.open("test.avi",fourcc,fps,S);
    }
    else
    {
        S = Size((width*shared_region)/(scale*100),height/scale);
//        writer.open("/run/user/1000/images/test.avi",fourcc,fps,S);
        writer.open("test.avi",fourcc,fps,S);
        S = Size(width-(width*shared_region)/100,height);
        //writer2.open("/run/user/1000/images/test2.avi",fourcc,fps,S);
        writer2.open("test2.avi",fourcc,fps,S);
    }
    while(val<iter){
		res=0;
 		auto t1 = std::chrono::high_resolution_clock::now();
        //printf("Acquiring frame id %0d\n",val+1);
		scaler = pow(2,(resolution_region[(val%4)]));
#ifdef switch_stream
    	if(ioctl(fd, VIDIOC_STREAMON, &type) < 0){
    	    perror("VIDIOC_STREAMON");
    	    exit(1);
    	}
#endif
        // The buffer's waiting in the outgoing queue.
	for(int val=0;val<1;val++) 
	{
        if(ioctl(fd, VIDIOC_QBUF, &bufferinfo) < 0){
            perror("VIDIOC_QBUF");
            exit(1);
        }
        if(ioctl(fd, VIDIOC_DQBUF, &bufferinfo) < 0){
            perror("VIDIOC_QBUF");
            exit(1);
    	}
	}
#ifdef switch_stream
     if(ioctl(fd, VIDIOC_STREAMOFF, &type) < 0){ perror("VIDIOC_STREAMOFF");
        exit(1);
     }
#endif
        val++;
        //control.id = V4L2_CID_TEST_PATTERN_RED;
        //control.value=res;
    /* Y//our loops end here. */
        //if(ioctl(fd, VIDIOC_S_CTRL, &control) < 0){
        //    perror("VDIOC_S_CTRL");
        //    exit(1);
        //}
        p[0] = CV_IMWRITE_JPEG_QUALITY;
        p[1] = 100;
        p[2] = 0;
		bayer = cvCreateImage(sz, IPL_DEPTH_8U, 1);
		bayer->imageData = (char *)(buffer_start);
		src = cvCreateImage(sz, IPL_DEPTH_8U, 1);
        cvCopy(bayer, src);
        if(shared_region==0){
            roi.x =0; //1200     // 950
            roi.y =0; //350      //150 
            roi.width = resx[0];  //2360          //2750
            roi.height = resy[0];  //1235 /2
		    dst = cvCreateImage({roi.width,roi.height}, IPL_DEPTH_8U, 3);
		    cvCvtColor(src, dst, CV_BayerRG2BGR);
		    cvReleaseImage(&src);
		    cvReleaseImage(&bayer);
            wFrame= cvarrToMat(dst);
            writer.write(wFrame); 
//            cv::imencode(".jpg", wFrame, buf, std::vector<int>());
//            writer<<dst;
		    //cvSaveImage(fn1, dst, p);
		    cvReleaseImage(&dst);
        }
        else{
    	    sprintf(fn, "/run/user/1000/images/m.jpg");
            roi.x =0; //1200     // 950
            roi.y =0; //350      //150 
            roi.width = int(resx[0]*shared_region/100);  //2360          //2750
            roi.height = resy[0];  //1235 
		    cvSetImageROI(src, roi);
		    src3 = cvCreateImage(cvSize(roi.width,roi.height), IPL_DEPTH_8U, 1);
            cvCopy(src, src3);
		    src1 = cvCreateImage(cvSize(roi.width/scale,roi.height/scale), IPL_DEPTH_8U, 1);
            cvResize(src3,src1,CV_INTER_LINEAR);
		    dst = cvCreateImage({roi.width/scale,roi.height/scale}, IPL_DEPTH_8U, 3);
		    cvCvtColor(src1, dst, CV_BayerRG2BGR);
		    cvReleaseImage(&src1);
		    cvReleaseImage(&bayer);
            wFrame= cvarrToMat(dst);
            writer.write(wFrame); 
		    //cvSaveImage(fn1, dst, p);
		    cvReleaseImage(&dst);
    	    sprintf(fn, "/run/user/1000/images/m2.jpg");
            roi.x =roi.width; //1200     // 950
            roi.width = resx[0]-roi.width;  //2360          //2750
		    cvSetImageROI(src, roi);
		    dst = cvCreateImage({roi.width,roi.height}, IPL_DEPTH_8U, 3);
		    cvCvtColor(src, dst, CV_BayerRG2BGR);
		    cvReleaseImage(&src);
		    cvReleaseImage(&bayer);
            wFrame= cvarrToMat(dst);
            writer2.write(wFrame); 
		    //cvSaveImage(fn1, dst, p);
		    cvReleaseImage(&dst);
        }
//        sleep(1);
		auto t2 = std::chrono::high_resolution_clock::now();
 		auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
        delay = 270000- float(duration);
    	std::cout << delay<<std::endl;
        usleep(delay);
 		//while( duration <250000){
		//	t2 = std::chrono::high_resolution_clock::now();
 		//	duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
		//}
     }
        writer.release();
    system("rm running.re");
     if(ioctl(fd, VIDIOC_STREAMOFF, &type) < 0){
        perror("VIDIOC_STREAMOFF");
        exit(1);
     }
     close(fd);
    return EXIT_SUCCESS;
}

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
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
//#define avoid_small
//#define switch_stream
#include <thread>
#include <ctime>
#include <sstream>
#include <fstream>
//#include <chrono>
//#define no_process
#define put_overlay
#include <opencv2/imgproc.hpp>
#define BUF_NO 150
int vblank;
using namespace std;
using namespace cv;
using namespace std::chrono;
ofstream myfile;
int fd;
vector<uchar> buf;  
int resolution_region[4]= {0,0,0,0};
int type;
void* buffer_start;
int delay;
int height=1080;
int width=1080;
int w1=1920;
int h1=1080;
bool shared_region_on_left=false; // position of shared region (left/right) in  the image frame
Size S;
const string filename="/run/user/1000/images/test.mp4";
int fourcc = VideoWriter::fourcc('M','J','P','G');
//int fourcc = VideoWriter::fourcc('M','P','E','G');
double fps= 4;
unsigned long time_now2 = -1;
unsigned long time_now = -1;
unsigned long prev_time = -1;
unsigned long times[BUF_NO];
unsigned long seqs[BUF_NO];
VideoWriter writer;
VideoWriter writer2;
IplImage *dst,*dst2,*src1,*src3,*to_process;
IplImage *src[BUF_NO];
Mat wFrame,wFrame3,wFrame5;
struct v4l2_control control;
struct v4l2_format formats;
struct v4l2_capability cap;
struct v4l2_requestbuffers bufrequest;
struct v4l2_buffer bufferinfo;
int resx[15] = { width, 1100, 900, 900, 800, 700,  600, 600, 512 , 440, 360, 224, 160, 96, 70};
int resy[15] = { height,  800,  900, 800, 800, 800 , 800, 600, 512 , 440, 360, 224, 160, 96, 70};
int shared_region=0;
int scale=3; 
int iter;
CvSize sz = { width, height };
cv::Rect roi;
IplImage *bayer, *rgb;
//int copied = 1;
int frame_ready= 0;
int done=0;
static int global_delay=100000 ;
static int gl_dl = global_delay/1000;
long dl;
int cap_nu =0;
long long int producer_pointer=0;
long long int consumer_pointer=0;
void capture()
{
    int val = 0;
    while(1)
    {
        //auto t1 = std::chrono::high_resolution_clock::now();
//	    auto now= std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
//        time_now2 = (unsigned long)now;
//        dl = (gl_dl-(time_now2 %gl_dl))%gl_dl;
        //usleep(dl*1000);
	    auto now= std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        time_now  = (unsigned long)now;
        //        if((int)((time_now-prev_time+3)/10)!=(int)(gl_dl/10)){
        //:wif(val<10)
           cout<<"duaration "<<time_now-prev_time<<" ms "<<time_now<<"frame number"<<val<<endl;
        prev_time= time_now;
        //cout <<"capture num" << time_now<<endl;
        if(1)
        {
		
    #ifdef switch_stream
        	if(ioctl(fd, VIDIOC_STREAMON, &type) < 0){
        	    perror("VIDIOC_STREAMON");
        	    exit(1);
        	}
    #endif
            // The buffer's waiting in the outgoing queue.
            // capture image here
            if(ioctl(fd, VIDIOC_QBUF, &bufferinfo) < 0){
                perror("VIDIOC_QBUF");
                exit(1);
            }
            if(ioctl(fd, VIDIOC_DQBUF, &bufferinfo) < 0){
                perror("VIDIOC_QBUF");
                exit(1);
        	}
    #ifdef switch_stream
            if(ioctl(fd, VIDIOC_STREAMOFF, &type) < 0){ 
               perror("VIDIOC_STREAMOFF");
               exit(1);
            }
    #endif
        //copied=0;
        #ifndef no_process
        if((producer_pointer-consumer_pointer)<BUF_NO){
            roi.x =0; //1200     // 950
            roi.y =0; //350      //150
            roi.width = resx[0];  //2360          //2750
            roi.height = resy[0];  //1235 /2
	    	bayer = cvCreateImage({w1,h1}, IPL_DEPTH_8U, 1);
	    	bayer->imageData = (char *)(buffer_start);  
	    	cvSetImageROI(bayer, roi);
                cout<<"cv copy capture "<<producer_pointer<<endl;
            cvCopy(bayer, src[producer_pointer%BUF_NO]);
                cout<<"cv copy capture done"<<endl;
            times[producer_pointer%BUF_NO]=time_now;
            seqs[producer_pointer%BUF_NO]=val;
                cout<<"cv copy relase"<<endl;
            cvReleaseImage(&bayer);
                cout<<"cv copy relase done"<<endl;
            producer_pointer+=1;
        }
        else {
                    cout<<"buff full "<<" "<<producer_pointer<<" "<<consumer_pointer<<endl;
        }
         #endif
        val+=1;
     }
   //  auto t2 = std::chrono::high_resolution_clock::now();
   //  auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
   //  delay = global_delay- int(duration);
   // std::cout<<"capture delay"<<delay<<std::endl;
    //if(delay<0){
    //    delay=0;
    //}
    // std::cout << delay<<std::endl;
  //   usleep(delay);
     if(done){
         break;
     }
   }
}
void process()
{
    int val = 1;
    unsigned long int time_stamp=0;
    while(val<iter+1) 
    {
        while(producer_pointer==consumer_pointer){
            usleep(1000);
        }
        auto t1 = std::chrono::high_resolution_clock::now();
    #ifndef no_process
        if(1)
        {

            time_stamp=times[consumer_pointer%BUF_NO];
            cap_nu=seqs[consumer_pointer%BUF_NO];
    	    myfile<<cap_nu<<endl<<flush;
	        to_process = cvCreateImage(sz, IPL_DEPTH_8U, 1);
            cvCopy(src[consumer_pointer%BUF_NO],to_process);
            consumer_pointer+=1;
            cout<<"producer "<<producer_pointer<<" consumer "<<consumer_pointer<<endl;


            if(shared_region==0){
	    	    dst = cvCreateImage({roi.width,roi.height}, IPL_DEPTH_8U, 3);
	    	    cvCvtColor(to_process, dst, CV_BayerRG2BGR);
                frame_ready=0;
                wFrame= cvarrToMat(dst);
                // add text overlay -- for debugging purpose
                #ifdef put_overlay
                    stringstream ss;
                    string str1 ;
                    ss << time_stamp;
                    ss >> str1;
                    cv::putText(wFrame,str1,cv::Point(30,30),cv::FONT_HERSHEY_DUPLEX,1,cv::Scalar(255,255,255),1,false);
               #endif
                writer.write(wFrame);
                cvReleaseImage(&dst);
                cvReleaseImage(&to_process);

            }
            else{
                // first write shared region image

                roi.y =0;
                roi.width = int((resx[0] * shared_region) / 100);
                roi.width -= roi.width%scale;
                roi.height = resy[0];

                if (shared_region_on_left == true) {
                    roi.x = 0;
                }
                else {
                // shared region on right side of image frame
                    roi.x = resx[0] - roi.width ;
                }

                // specific experiment where only one camera captures shared region

                #ifndef avoid_small
                cout<<"set roi 1"<<producer_pointer%BUF_NO<< " "<<consumer_pointer%BUF_NO<<endl;
                    cvSetImageROI(to_process, roi);
                    src3 = cvCreateImage(cvSize(roi.width,roi.height), IPL_DEPTH_8U, 1);
                    src1 = cvCreateImage(cvSize(roi.width/scale,roi.height/scale), IPL_DEPTH_8U, 1);
                    dst = cvCreateImage({roi.width/scale,roi.height/scale}, IPL_DEPTH_8U, 3);
                    cvCopy(to_process, src3);
                    cvResize(src3,src1,CV_INTER_LINEAR);
                    if(scale==3){
                        cvCvtColor(src1, dst, CV_BayerBG2BGR);
                    }
                    else{
                        cvCvtColor(src1, dst, CV_BayerRG2BGR);
                    }
                    cvReleaseImage(&src3);  
                    cvReleaseImage(&src1);  
                    wFrame= cvarrToMat(dst);
                    writer.write(wFrame);
                    cvReleaseImage(&dst);  
                #endif
                // write non-shared region image

                roi.y = 0;
                if (shared_region_on_left == true){

                    roi.x = roi.width;
                }
                else{
                    roi.x = 0;
                }

                cout<<"set roi 2"<<producer_pointer%BUF_NO<<endl;
                roi.width = resx[0]-roi.width;
	    	    cvSetImageROI(to_process, roi);
                src3 = cvCreateImage(cvSize(roi.width,roi.height), IPL_DEPTH_8U, 1);
                cvCopy(to_process, src3);
	    	    dst2 = cvCreateImage({roi.width,roi.height}, IPL_DEPTH_8U, 3);
	    	    cvCvtColor(src3, dst2, CV_BayerRG2BGR);
                wFrame= cvarrToMat(dst2);
                writer2.write(wFrame); 
                cvReleaseImage(&dst2);
                cvReleaseImage(&to_process);
                cvReleaseImage(&src3);  
            }
         #else
            frame_ready=0;
         #endif
            val+=1;
        }
        
     auto t2 = std::chrono::high_resolution_clock::now();
     auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
    // delay = global_delay- int(duration);
    std::cout<<"process duration"<<duration/1000<<std::endl;
    }
    done=1;
}
void setup()
{
    myfile.open ("synch.txt");
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
    formats.fmt.pix.pixelformat = V4L2_PIX_FMT_SBGGR8;
    formats.fmt.pix.width = w1;
    formats.fmt.pix.height = h1;
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
    if(shared_region==0){
        S = Size(width,height);
  //      writer.open("/run/user/1000/images/test.avi",fourcc,fps,S);
        writer.open("test.avi",fourcc,fps,S);
    }
    else
    {
       int ww= (((width*shared_region)/100)-(((width*shared_region)/100)%scale));
        S = Size(ww/scale,height/scale);
    //    writer.open("/run/user/1000/images/test.avi",fourcc,fps,S);
        writer.open("test.avi",fourcc,fps,S);
        S = Size(width-ww,height);
    //    writer2.open("/run/user/1000/images/test2.avi",fourcc,fps,S);
        writer2.open("test2.avi",fourcc,fps,S);
    }
    control.id = V4L2_CID_VBLANK;
     control.value=vblank;
    /* Your loops end here. */
        if(ioctl(fd, VIDIOC_S_CTRL, &control) < 0){
            perror("VDIOC_S_CTRL");
            exit(1);
        }
    for(int i=0; i<BUF_NO; i++){
	    src[i] = cvCreateImage(sz, IPL_DEPTH_8U, 1);
    }
}

int main(int argc, char** argv){
        static int align=10000;
    // read cmd line args
    shared_region=atoi(argv[1]);
    iter=atoi(argv[2]);
    vblank=atoi(argv[3]);
    shared_region_on_left=atoi(argv[4]);
    scale=atoi(argv[5]);

    // setup the system
    system("echo r> running.re");
    setup();

	    auto now= std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        time_now2 = (unsigned long)now;
        dl = (align-(time_now2 %align))%align;
        usleep(dl*1000);
	    now= std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        time_now2 = (unsigned long)now;
    cout << "starting time "<<time_now2<<endl;

    #ifndef switch_stream
        	if(ioctl(fd, VIDIOC_STREAMON, &type) < 0){
        	    perror("VIDIOC_STREAMON");
        	    exit(1);
        	}
    #endif
	    thread th1(capture);
	    thread th2(process);
        th1.join();
        th2.join();
    #ifndef switch_stream
        	if(ioctl(fd, VIDIOC_STREAMOFF, &type) < 0){
        	    perror("VIDIOC_STREAMOFF");
        	    exit(1);
        	}
    #endif
    system("rm running.re");
    writer.release();
    writer2.release();
    close(fd);
    return EXIT_SUCCESS;
}

#include <stdlib.h>
#include <stdio.h>
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
#define jpeg
#include <cv.h>
#include <highgui.h>
#define switch_stream
int main(void){
    int fd;
    int val = 0;
	int scaler=1;
	int resolution_region[4]= {0,0,0,0};
	int type;
	IplImage *src,*dst;
    struct v4l2_control control;
    struct v4l2_format format;
    struct v4l2_capability cap;
    struct v4l2_requestbuffers bufrequest;
    struct v4l2_buffer bufferinfo;
	int resx[15] = { 1200, 1100, 900, 900, 800, 700,  600, 600, 512 , 440, 360, 224, 160, 96, 70};
    int resy[15] = { 800,  800,  900, 800, 800, 800 , 800, 600, 512 , 440, 360, 224, 160, 96, 70};
	int res=0;

	CvSize sz = { 1920, 1080 };
	cv::Rect roi;
	IplImage *bayer, *rgb;
    int jpgfile;
    char fn[32];
	char *fn1;
	fn1 = (char *)&fn;
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
    while(val<15){
		res=val;
 		auto t1 = std::chrono::high_resolution_clock::now();
        printf("Acquiring frame id %0d\n",val+1);
		scaler = pow(2,(resolution_region[(val%4)]));
    	sprintf(fn, "/run/user/1000/images/m.jpg");
#ifdef switch_stream
    	if(ioctl(fd, VIDIOC_STREAMON, &type) < 0){
    	    perror("VIDIOC_STREAMON");
    	    exit(1);
    	}
#endif
        // The buffer's waiting in the outgoing queue.
	for(int val=0;val<4;val++) 
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
        control.id = V4L2_CID_TEST_PATTERN_RED;
        control.value=res;
    /* Your loops end here. */
        if(ioctl(fd, VIDIOC_S_CTRL, &control) < 0){
            perror("VDIOC_S_CTRL");
            exit(1);
        }
		#ifdef jpeg
        roi.x =0; //1200     // 950
        roi.y =0; //350      //150 
        roi.width = resx[res];  //2360          //2750
        roi.height =resy[res];  //1235 /2
		bayer = cvCreateImage(sz, IPL_DEPTH_8U, 1);
		bayer->imageData = (char *)(buffer_start);
		src = cvCreateImage({roi.width,roi.height}, IPL_DEPTH_8U, 1);
		cvSetImageROI(bayer, roi);
        cvCopy(bayer, src);
		dst = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 3);
		cvCvtColor(src, dst, CV_BayerRG2BGR);
		cvReleaseImage(&src);
		cvReleaseImage(&bayer);
		cvSaveImage(fn1, dst, 0);
		cvReleaseImage(&dst);
		system("echo test > /run/user/1000/images/test.txt");
		auto t2 = std::chrono::high_resolution_clock::now();
 		auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
 		while( duration <250000){
			t2 = std::chrono::high_resolution_clock::now();
 			duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
		}
    	std::cout << duration<<std::endl;
		#endif
     }
     if(ioctl(fd, VIDIOC_STREAMOFF, &type) < 0){
        perror("VIDIOC_STREAMOFF");
        exit(1);
     }
     close(fd);
    return EXIT_SUCCESS;
}

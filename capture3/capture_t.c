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
int resx[15] = { 1200, 1100, 900, 900, 800, 700,  600, 600, 512 , 440, 360, 224, 160, 96, 70};
int resy[15] = { 800,  800,  900, 800, 800, 800 , 800, 600, 512 , 440, 360, 224, 160, 96, 70};
int res=14;
int val=0;
char fn[32];
#define switch_stream 
int main(void)
{ 
	IplImage *bayer;
	char image_data[1200*800];
	 long int durations[100];	
		for(int pix=0; pix<resx[res]*resy[res];pix++)
		{
			image_data[pix]=pix%256;	
		}
		while (val<10) {
			sprintf(fn, "test.ppm");
			auto t1 = std::chrono::high_resolution_clock::now();
			bayer = cvCreateImage({resx[res],resy[res]}, IPL_DEPTH_8U, 1);
			bayer->imageData = (char *)(image_data);
			cvSaveImage(fn, bayer, 0);
			cvReleaseImage(&bayer);
			auto t2 = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
			durations[val] = (long)duration;
			//while( duration <250000){
			//t2 = std::chrono::high_resolution_clock::now();
			//	duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
			//}
			//std::cout << duration<<std::endl;
			val++;
		}
		long int sum=0;
		val=0;
		while (val<10) {
			sum = sum+durations[val];
			val=val+1;
		}
		printf("duration %0d\n",sum/100);
		return 0;
}


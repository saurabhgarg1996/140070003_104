/*************************************************************************************
CS 101: LIBRARY HELPMATE FIRE BIRD V BOT
Team ID : 104
Team Members :
(I) Saurabh Garg
(II)Shaurya Arya
(III)Pratik Brahma
(IV)Abhin Shah
Filename : Image processing Code
************************************************************************************/
#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include<math.h>
using namespace cv;
using namespace std;
/** to detect colour of book */
char return_color(int b,int g,int r)
{
if(b>r&&b>g)
return ' b'; /** returns blue */
else if(r>g&&r>b)
return 'r'; /** returns red */
else if(g>r&&g>b)
return 'g'; /** returns green */
}
/** A Simple Camera Capture Framework */
int captureimg() {
CvCapture* capture = cvCaptureFromCAM( CV_CAP_ANY );
if ( !capture ) {
fprintf( stderr, "ERROR: capture is NULL \n" );
getchar();
return -1;
}
/** Create a window in which the captured images will be presented */
cvNamedWindow( "mywindow", CV_WINDOW_AUTOSIZE );
/** Show the image captured from the camera in the window and repeat */
while ( 1 ) {
/** Get one frame */
IplImage* frame = cvQueryFrame( capture );
if ( !frame ) {
fprintf( stderr, "ERROR: frame is null...\n" );
getchar();
break;
}
cvShowImage( "mywindow", frame );
/** Do not release the frame! */
if ( (cvWaitKey(10) & 255) == 's' ) {
CvSize size = cvGetSize(frame);
IplImage* img= cvCreateImage(size, IPL_DEPTH_16S, 1);
img = frame;
cvSaveImage("colour.jpg",img);
}
if ( (cvWaitKey(10) & 255) == 27 ) break;
}
/** Release the capture device housekeeping */
cvReleaseCapture( &capture );
cvDestroyWindow( "mywindow" );
return 0;
}
int main( int argc, const char** argv )
{
captureimg();
Mat img = imread("colour.jpg", CV_LOAD_IMAGE_COLOR); /**read the image data in the file */
"MyPic.JPG" and store it in 'img'
if (img.empty()) /**check whether the image is loaded or not */
{
cout << "Error : Image cannot be loaded..!!" << endl;
/** system("pause"); wait for a key press */
return -1;
}
const int row=img.rows;
const int col=img.cols;
int r[row][col],g[row][col],b[row][col];
for(int i = 0; i < img.rows; i++)
{
for(int j = 0; j < img.cols; j++)
{
b[i][j] = img.at<cv::Vec3b>(i,j)[0];
g[i][j] = img.at<cv::Vec3b>(i,j)[1];
r[i][j] = img.at<cv::Vec3b>(i,j)[2];
}
}
int center[3],flag=0;
center[0]=b[row/2][col/2];
center[1]=g[row/2][col/2];
center[2]=r[row/2][col/2];
for(int i = 0; i < img.rows; i++)
{
for(int j = 0; j < img.cols; j++)
{
if((abs(center[0]-b[i][j])<20)&&(abs(center[1]-g[i][j])<20)&&(abs(center[2]-r[i][j])<20))
flag++;
}
}
if(flag>=(row*col/9))
cout<<return_color(center[0],center[1],center[2]);
namedWindow("MyWindow", CV_WINDOW_AUTOSIZE); /**create a window with the name*/
"MyWindow"
imshow("MyWindow", img); /**display the image which is stored in the 'img' in the "MyWindow" */
window
waitKey(0); /**wait infinite time for a keypress */
destroyWindow("MyWindow"); /** destroy the window with the name, "MyWindow" */
return 0;
}

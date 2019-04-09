#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>
#include "opencv2\opencv.hpp"
#include "opencv2\highgui.hpp"
#include "opencv2\video\background_segm.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\core\core.hpp"
#include "opencv2\video.hpp"
#include "opencv2\videoio.hpp"
#include "opencv2\imgcodecs.hpp"
#include "opencv2\imgproc.hpp"
#include <iostream>

#pragma comment(lib, "C:\\opencv\\build\\x64\\vc15\\lib\\opencv_world343.lib")
#pragma comment(lib, "C:\\opencv\\build\\x64\\vc15\\lib\\opencv_world343d.lib")

using namespace cv;
using namespace std;

//create an image to store the video screen grab
Mat frame; //current frame
Mat grayFrame;
Mat HSVframe;
Mat contourOutput;
Mat CannyEdge;
Mat cloneFrame;
Mat linesFrame;
Mat fgMaskMOG2; //fg mask generated by MOG2 method
Mat dilFrame;
Mat openingFrame;
Mat croppedFrame;
Mat OutputFrame;
Mat testFrame;
Mat filteredHSV;
Mat GuassianBlurFrame;
Rect roi(475, 0, 350, 500);
std::vector<std::vector<cv::Point> > contours;
std::vector<std::vector<cv::Point> > contours_poly(contours.size());
//std::vector<Rect> boundRect(contours.size());
std::vector<Point2f>center(contours.size());
std::vector<float>radius(contours.size());
std::vector<Vec4i> hierarchy;
Ptr<BackgroundSubtractor> pMOG2; //MOG2 Background subtractor
size_t i = 0;
Scalar minRGB(0, 0, 200);
Scalar maxRGB(0, 0, 255);
int H_min = 98, H_max = 179, S_min = 62, S_max = 255, V_min = 155, V_max = 255;
int IDRmin = 1;
int IDRmax = 2;
int IDGmin = 3;
int IDGmax = 4;
int IDBmin = 5;
int IDBmax = 6;
int cannyLow = 30;
int cannyHigh = 60;
int dilIterate = 1;
int Houghthreshold = 8;
int minLen = 3;
int maxGap = 4;
double alpha = 0.5;
double beta = 1 - alpha;
double frameWidth = 0.0;
double frameHeight = 0.0;
int thrshValue = 244;
int maxVal = 500;
int MedianBlurKernel = 3;

Scalar min(175, 175, 250);	//white min for RGB
Scalar max(255, 255, 255);	//white max for RGB
Mat threshold_frame;
Mat str_el = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
std::vector<Vec4i> lines; // will hold the results of the detection						 

int MAX_KERNEL_LENGTH = 31;

int main()
{
	
	//setup the video capture method using the default camera
	VideoCapture cap;
	cap.open("CarDashCameraFootage.mp4");
	
	namedWindow("VideoCaptureTutorial", WINDOW_AUTOSIZE);
	namedWindow("Control", WINDOW_AUTOSIZE);
	createTrackbar("Hue Min", "Control", &H_min, 179);
	createTrackbar("Sat Min", "Control", &S_min, 255);
	createTrackbar("Value Min", "Control", &V_min, 255);
	createTrackbar("Hue Max", "Control", &H_max, 179);
	createTrackbar("Sat Max", "Control", &S_max, 255);
	createTrackbar("Value Max", "Control", &V_max, 255);
	createTrackbar("Threshold Value", "Control", &thrshValue, 500);
	createTrackbar("Max. Value", "Control", &maxVal, 500);
	createTrackbar("Dil Iterations", "Control", &dilIterate, 10);
	createTrackbar("Low threshold", "Control", &cannyLow, 255);
	createTrackbar("High threshold", "Control", &cannyHigh, 255);
	createTrackbar("Line Threshold", "Control", &Houghthreshold, 255);
	createTrackbar("Min Length", "Control", &minLen, 100);
	createTrackbar("Max Gap", "Control", &maxGap, 100);
	//createTrackbar("Median Blur", "Control", &MedianBlurKernel, 100);
	resizeWindow("Control", 400, 300);

	pMOG2 = createBackgroundSubtractorMOG2();

	while (1)
	{
		//grad a frame from the video camers
		cap >> frame;
		
		//Convert RGB frame to gray scale
		cvtColor(frame, grayFrame, CV_RGB2GRAY);

		//Convert frame to HSV
		cvtColor(frame, HSVframe, CV_RGB2HSV);//CV_BGR2YUV);
		
		//Dilate
		inRange(HSVframe, Scalar(H_min, S_min, V_min), Scalar(H_max, S_max, V_max), filteredHSV);
		
		threshold(grayFrame, threshold_frame, double(thrshValue), double(maxVal), THRESH_BINARY);
		addWeighted(threshold_frame, alpha, filteredHSV, beta, 2.0, testFrame);
		
		medianBlur(testFrame, testFrame, MedianBlurKernel);
		dilate(testFrame, dilFrame, Mat(), Point(-1, -1), dilIterate, BORDER_CONSTANT, 1);	//

		//Apply opening on the grey frame
		morphologyEx(dilFrame, dilFrame, cv::MORPH_OPEN, str_el);
		morphologyEx(dilFrame, dilFrame, cv::MORPH_CLOSE, str_el);

		//Canny edge detection
		Canny(dilFrame, CannyEdge, double(cannyLow), double (cannyHigh), 3);
		cvtColor(CannyEdge, linesFrame, CV_GRAY2BGR);
		//imshow("Canny-Grey", CannyEdge);

		HoughLinesP(CannyEdge, lines, 1, CV_PI / 180, Houghthreshold, minLen, maxGap);
		for(i = 0; i < lines.size(); i++)
		{
			Vec4i l = lines[i];
			line(frame, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255, 0, 0), 3, CV_AA);
			
		}		
		imshow("Hough lines", frame);//linesFrame);		
		printf("Current config: %d %d %d %d %d %d itr: %d thrld:%d Min len:%d Ma gap: %d L:%d H:%d\n",
			H_min, S_min, V_min, H_max, S_max, V_max, dilIterate, Houghthreshold, minLen, maxGap,cannyLow, cannyHigh);
		
		//create a 50ms delay
		waitKey(50);
	}

	return 0;
}
/*OpenCV Lane Detection Simple Demo - By: Karim El-Rayes									*/
/*Notes: If Linker errors occur on Visual Studio, use "Release" option instead of "Deubg".	*/
/*Don't forget to the add include path: <drive>:\opencv\build\include						*/

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

//Linking libraries
#pragma comment(lib, "opencv_world343.lib")
#pragma comment(lib, "opencv_world343d.lib")

using namespace cv;
using namespace std;

int main()
{
	//create an image to store the video screen grab
	Mat frame; 
	Mat grayFrame;
	Mat HSVframe;
	Mat CannyEdge;
	Mat cloneFrame;
	Mat linesFrame;
	Mat dilFrame;
	size_t i = 0;
	
	Mat threshold_frame;
	Mat str_el = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
	std::vector<Vec4i> lines; // will hold the results of the detection

	VideoCapture cap;
	cap.open("CarDashCameraFootage.mp4");
	
	while (1)
	{
		cap >> frame;
				
		//Convert frame to HSV
		cvtColor(frame, HSVframe, CV_RGB2HSV);
		//imshow("HSV frame", HSVframe);


		inRange(HSVframe, Scalar(0, 0, 155), Scalar(179, 255, 255), threshold_frame);
		dilate(threshold_frame, dilFrame, Mat(), Point(-1, -1), 4, BORDER_CONSTANT, 1);	//
		//Apply opening on the grey frame
		morphologyEx(dilFrame, dilFrame, cv::MORPH_OPEN, str_el);
		morphologyEx(dilFrame, dilFrame, cv::MORPH_CLOSE, str_el);
		//imshow("Dilated frame", dilFrame);

		morphologyEx(threshold_frame, threshold_frame, cv::MORPH_OPEN, str_el);
		morphologyEx(threshold_frame, threshold_frame, cv::MORPH_CLOSE, str_el);

		//Canny edge detection
		Canny(threshold_frame, CannyEdge, 10, 100, 3);
		cvtColor(CannyEdge, linesFrame, CV_GRAY2BGR);
		
		
		HoughLinesP(CannyEdge, lines, 1, CV_PI/180, 35, 5, 2);
		for(i = 0; i < lines.size(); i++)
		{
			Vec4i l = lines[i];
			line(frame, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255, 0, 0), 3, CV_AA);
			
		}
		imshow("Hough lines", frame);
		
		//create a 33ms delay
		waitKey(20);
	}

	return 0;
}
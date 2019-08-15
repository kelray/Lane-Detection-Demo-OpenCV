/*
 * Copyright 2018, 2019 Karim El-Rayes. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY HONG XU ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL HONG XU OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are
 * those of the authors and should not be interpreted as representing official
 * policies, either expressed or implied, of Hong Xu.
 *
 *
 *
 *
 *
 * Author: Karim El-Rayes
 *
 *
 * OpenCV Lane Detection Simple Demo - 									
 * Notes: If Linker errors occur on Visual Studio, use "Release" option instead of "Deubg".	
 * Don't forget to the add include path: <drive>:\opencv\build\include						
*/

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

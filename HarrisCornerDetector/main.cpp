///@File: main.cpp
///@Brief: Contains implementation of entry point of the application
///@Author: Stepan Sidorov
///@Date: 05 October 2015

#include "stdafx.h"
#include "CannyEdgeDetector.h"
#include "HarrisCornerDetector.h"

///@brief Entry point
int _tmain(int argc, char** argv)
{
   CannyEdgeDetector cannyDemo;
   HarrisCornerDetector harrisDemo;
   harrisDemo.Show(cv::String(argv[1]));
//   cannyDemo.Show(cv::String(argv[1]));
   return 0;
}

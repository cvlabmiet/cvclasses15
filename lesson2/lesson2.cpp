///@File: main.cpp
///@Brief: Contains implementation of entry point of the application
///@Author: Svetlana Kuksova
///@Date: 21 September 2015

#include "stdafx.h"

#include "CannyEdgeDetector.h"
#include "HarrisCornerDetector.h"

///@brief Entry point
int main(int argc, char** argv)
{
   CannyEdgeDetector cannyDemo;
   HarrisCornerDetector harrisDemo;
   cannyDemo.Show(cv::String(argv[1]));
//   harrisDemo.Show(cv::String(argv[1]));
   return 0;
}

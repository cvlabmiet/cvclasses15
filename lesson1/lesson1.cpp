///@File: main.cpp
///@Brief: Contains implementation of entry point of the application
///@Author: Roman Golovanov
///@Date: 08 September 2015

#include "stdafx.h"

#include "LaplacianEdgeDetector.h"
#include <iostream>
///@brief Entry point
int main(int argc, char** argv )
{
	LaplacianEdgeDetector laplacianDemo;

	laplacianDemo.Show(cv::String(argv[1]));
	return 0;
}

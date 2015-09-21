// lesson2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>


#include "CannyEdgeDetector.h"
#include <iostream>
///@brief Entry point
int main(int argc, char** argv)
{
	CannyEdgeDetector CannyDemo;
	CannyDemo.Show(cv::String(argv[1]));
	getchar();
	return 0;
}

///@File: HarrisCornerDetector.h
///@Brief: Contains definition of HarrisCornerDetector class
///@Author: Stepan Sidorov
///@Date: 05 October 2015

#pragma once

#include "stdafx.h"

///@class HarrisCornerDetector
///@brief Demonstrates the Harris corner detector
class HarrisCornerDetector
{
public:
	///@brief ctor
	HarrisCornerDetector();

	///@brief Launch demonstration for passed image
	///@param imgPath, in - the path to the image to be processed
	void Show(const cv::String& imgPath);

	///@brief Returns the string with full name of this detector
	static cv::String ReplyName()
	{
		return "Harris Corner Detector";
	}

private:
	///@brief applies algorithm according to the passed data
	///@see cv::TrackbarCallback
	static void detectFeatures(int pos, void* data);

private:
	///@brief Contains main configurable parametrs for demo
	struct Params
	{
		int windowSize;				// size of window
		int gaussianSigma;			// sigma of Gaussian
		int parameterK;				// value of K*100 (because of too small values for integer type)
		int threshold;				// threshold value

		cv::String windowName;		// name of the window to be used for showing result image
		cv::Mat srcImage;			// source image
	};

	///@brief current parameters
	Params m_param;
};

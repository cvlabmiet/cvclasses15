///@File: LaplacianEdgeDetector.h
///@Brief: Contains definition of LaplacianEdgeDetector class
///@Author: Roman Golovanov
///@Date: 08 September 2015

#pragma once

#include "opencv2/opencv.hpp"

///@class LaplacianEdgeDetector
///@brief Demonstrates the laplacian of gaussian edge detector
class LaplacianEdgeDetector
{
public:
	///@brief ctor
	LaplacianEdgeDetector();

	///@brief Launch demonstration for passed image
	///@param imgPath, in - the path to the image to be processed
	void Show(const cv::String& imgPath);

	///@brief Returns the string with full name of this detector
	static cv::String ReplyName()
	{
		return "Laplacian Edge Detector";
	}

private:
	///@brief applies algorithm according to the passed data
	///@see cv::TrackbarCallback
	static void applyFilter(int pos, void* data);

private:

	///@brief Contains main configurable parametrs for demo
	struct Params
	{
		int gaussianKernelSize;		///< size of gaussian kernel NxN
		int gaussianSigma;			///< sigma of gaussian

		int laplacianKernelSize;	///< size of laplacian kernel NxN
		int laplacianScale;			///< scale of laplacian
		int laplacianDelta;			///< delta of laplacian

		cv::String windowName;		///< name of the window to be used for showing result image
		cv::Mat srcImage;			///< source image
	};

	///@brief current parameters
	Params m_param;
};

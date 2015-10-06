///@File: CannyEdgeDetector.h
///@Brief: Contains definition of CannyEdgeDetector class
///@Author: Svetlana Kuksova
///@Date: 21 September 2015

#pragma once

///@class CannyEdgeDetector
///@brief Demonstrates the Canny edge detector
class CannyEdgeDetector
{
public:
	///@brief ctor
	CannyEdgeDetector();

	///@brief Launch demonstration for passed image
	///@param imgPath, in - the path to the image to be processed
	void Show(const cv::String& imgPath);

	///@brief Returns the string with full name of this detector
	static cv::String ReplyName()
	{
		return "Canny Edge Detector";
	}

private:
	///@brief applies algorithm according to the passed data
	///@see cv::TrackbarCallback
	static void applyFilter(int pos, void* data);

private:

	///@brief Contains main configurable parametrs for demo
	struct Params
	{
		int blurringKernelSize;		///< size of blurring kernel NxN

		int lowThreshold;				///< first threshold
		int ratio;                  			///< coefficient
		int sobelKernel_size;			///< size of Sobel operator kernel

		cv::String windowName;		///< name of the window to be used for showing result image
		cv::Mat srcImage;			///< source image
	};

	///@brief current parameters
	Params m_param;
};
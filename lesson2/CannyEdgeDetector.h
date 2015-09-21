#pragma once

#include "opencv2/opencv.hpp"


class CannyEdgeDetector
{
public:

	CannyEdgeDetector();

	void Show(const cv::String& imgPath);
	static cv::String ReplyName(){
		return "Canny Edge Detector";
	}

private:
	static void applyFilter(int pos, void* data);

private:

	///@brief Contains main configurable parametrs for demo
	struct Params
	{
		int blurringKernelSize;		///< size of blurring kernel NxN

		int lowThreshold;			///< first threshold
		int ratio;                  ///< coefficient
		int sobelKernel_size;			///< size of Sobel operator kernel

		cv::String windowName;		///< name of the window to be used for showing result image
		cv::Mat srcImage;			///< source image
	};

	Params m_param;
};
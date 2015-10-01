///@File: CannyEdgeDetector.h
///@Brief: Contains implementation of CannyEdgeDetector class
///@Author: Svetlana Kuksova
///@Date: 21 September 2015

#include "stdafx.h"

#include "CannyEdgeDetector.h"

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

///////////////////////////////////////////////////////////////////////////////
CannyEdgeDetector::CannyEdgeDetector()
{
	static const Params defaultParams = {
		3, 0,
		3, 3,
		"New window"
	};

	m_param = defaultParams;
}

void CannyEdgeDetector::applyFilter(int pos, void* data)
{
	const Params& userData = *static_cast<Params*>(data);

	/// Reduce noise with a kernel 3x3
	cv::Mat bluredImg;
	cv::blur(userData.srcImage, bluredImg, cv::Size(userData.blurringKernelSize, userData.blurringKernelSize));

	/// Convert the image to grayscale
	cv::Mat gray;
	cvtColor(bluredImg, gray, CV_RGB2GRAY);

	/// Canny detector
	cv::Mat detected_edges;
	Canny(gray, detected_edges, userData.lowThreshold, userData.lowThreshold*userData.ratio, userData.sobelKernel_size);

	/// Using Canny's output as a mask, we display our result
	cv::Mat abs_dst;
	convertScaleAbs(detected_edges, abs_dst);
	cv::imshow(userData.windowName, abs_dst);
}
///////////////////////////////////////////////////////////////////////////////
void CannyEdgeDetector::Show(const cv::String& imgPath)
{
	/// Load an image
	m_param.srcImage = cv::imread(imgPath);
	resize(m_param.srcImage, m_param.srcImage, cv::Size(), 0.5, 0.5, cv::INTER_LINEAR);
	if (!m_param.srcImage.data){
		return;
	}

	/// Create window with original image
	cv::namedWindow(imgPath, CV_WINDOW_NORMAL);
	cv::imshow(imgPath, m_param.srcImage);

	/// Create DEMO window
	m_param.windowName = imgPath + " " + ReplyName();
	cv::namedWindow(m_param.windowName, CV_WINDOW_AUTOSIZE);


	/// Create a Trackbar for user to enter threshold
	int max_lowThreshold = 100;
	cv::createTrackbar("Min Threshold:", m_param.windowName, &m_param.lowThreshold, max_lowThreshold, applyFilter, static_cast<void*>(&m_param));

	cv::waitKey(0);
}
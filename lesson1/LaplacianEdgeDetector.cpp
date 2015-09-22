///@File: LaplacianEdgeDetector.cpp
///@Brief: Contains implementation of LaplacianEdgeDetector class
///@Author: Roman Golovanov
///@Date: 08 September 2015

#include "stdafx.h"

#include "LaplacianEdgeDetector.h"

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

///////////////////////////////////////////////////////////////////////////////
LaplacianEdgeDetector::LaplacianEdgeDetector()
{
	static const Params defaultParams = {
		3, 0,
		3, 1, 15,
		"New window"
	};

	m_param = defaultParams;
}

///////////////////////////////////////////////////////////////////////////////
void LaplacianEdgeDetector::Show(const cv::String& imgPath)
{
	/// Load an image
	m_param.srcImage = cv::imread(imgPath);
	if (!m_param.srcImage.data)
	{
		return;
	}

	/// Create window with original image
	cv::namedWindow(imgPath, CV_WINDOW_AUTOSIZE);
	cv::imshow(imgPath, m_param.srcImage);

	/// Create DEMO window
	m_param.windowName = imgPath + " " + ReplyName();
	cv::namedWindow(m_param.windowName, CV_WINDOW_AUTOSIZE);

	cv::createTrackbar("GKernel",
		m_param.windowName, &m_param.gaussianKernelSize, 15, applyFilter, static_cast<void*>(&m_param));
	cv::createTrackbar("GSigma",
		m_param.windowName, &m_param.gaussianSigma, 15, applyFilter, static_cast<void*>(&m_param));

	cv::createTrackbar("LKernel",
		m_param.windowName, &m_param.laplacianKernelSize, 15, applyFilter, static_cast<void*>(&m_param));
	cv::createTrackbar("LScale",
		m_param.windowName, &m_param.laplacianScale, 15, applyFilter, static_cast<void*>(&m_param));
	cv::createTrackbar("LDelta",
		m_param.windowName, &m_param.laplacianDelta, 15, applyFilter, static_cast<void*>(&m_param));

	cv::waitKey(0);
}

///////////////////////////////////////////////////////////////////////////////
// static 
void LaplacianEdgeDetector::applyFilter(int pos, void* data)
{
	const Params& userData = *static_cast<Params*>(data);

	if (userData.gaussianKernelSize % 2 == 0 ||
		userData.laplacianKernelSize % 2 == 0)
	{
		// skip invalid values
		return;
	}

	/// Remove noise by blurring with a Gaussian filter
	cv::Mat bluredImg;
	cv::GaussianBlur(userData.srcImage, bluredImg,
		cv::Size(userData.gaussianKernelSize, userData.gaussianKernelSize),
		userData.gaussianSigma,
		userData.gaussianSigma,
		cv::BORDER_DEFAULT);

	cv::Mat gray;
	cvtColor(bluredImg, gray, CV_RGB2GRAY);

	/// Apply Laplace function
	cv::Mat dst;
	cv::Laplacian(gray, dst,
		CV_16S,
		userData.laplacianKernelSize,
		userData.laplacianScale,
		userData.laplacianDelta,
		cv::BORDER_DEFAULT);

	cv::Mat abs_dst;
	convertScaleAbs(dst, abs_dst);
	cv::imshow(userData.windowName, abs_dst);
}
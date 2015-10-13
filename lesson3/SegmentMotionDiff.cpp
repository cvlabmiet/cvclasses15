///@File: SegmentMotionDiff.cpp
///@Brief: Contains implementation of SegementMotionDiff class
///@Author: Vitaliy Baldeev
///@Date: 01 October 2015

#include "SegmentMotionDiff.h"

#include "opencv2\videoio.hpp"
#include "opencv2\imgproc.hpp"
#include "opencv2\highgui.hpp"

///////////////////////////////////////////////////////////////////////////////
cv::Mat SegmentMotionDiff::process(cv::VideoCapture& capture)
{
    cv::Mat currentFrame;

    capture >> currentFrame;

    if (!m_backgroundUpdated)
    {
        cv::cvtColor(currentFrame, m_background, CV_RGB2GRAY);
        m_backgroundUpdated = true;
    }
    
    cv::Mat grayCurrentFrame;
    cv::cvtColor(currentFrame, grayCurrentFrame, CV_RGB2GRAY);

    cv::Mat result = abs(m_background - grayCurrentFrame);
    cv::threshold(result, result, m_threshold, 255, CV_THRESH_BINARY);

    return result;
}

///////////////////////////////////////////////////////////////////////////////
void SegmentMotionDiff::createGUI()
{
    const std::string windowName = GetName();
    cv::namedWindow(windowName);

    int initTrackbarValue = 10;
    setThresholdFromSlider(initTrackbarValue, &m_threshold);
    cv::createTrackbar("Threshold", windowName, &initTrackbarValue, 255, setThresholdFromSlider, &m_threshold);
}


///////////////////////////////////////////////////////////////////////////////
void SegmentMotionDiff::setThresholdFromSlider(int thresholdSlider, void* paramsPtr)
{
    int* data = static_cast<int*>(paramsPtr);
    *data = thresholdSlider;
}

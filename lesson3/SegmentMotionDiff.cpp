///@File: SegmentMotionDiff.cpp
///@Brief: Contains implementation of SegementMotionDiff class
///@Author: Vitaliy Baldeev
///@Date: 01 October 2015

#include "SegmentMotionDiff.h"

///////////////////////////////////////////////////////////////////////////////
std::string SegmentMotionDiff::GetName() const
{
    return m_algorithmName;
}

///////////////////////////////////////////////////////////////////////////////
void SegmentMotionDiff::apply(cv::Mat& currentFrame, cv::Mat& foreground)
{
    if (!m_backgroundUpdated)
    {
        updateBackground(currentFrame);
        m_backgroundUpdated = true;
    }
    
    cv::Mat grayCurrentFrame;
    cv::cvtColor(currentFrame, grayCurrentFrame, CV_RGB2GRAY);

    foreground = abs(m_background - grayCurrentFrame);
    cv::threshold(foreground, foreground, m_threshold, 255, CV_THRESH_BINARY);
}

///////////////////////////////////////////////////////////////////////////////
void SegmentMotionDiff::createGUI()
{
    cv::namedWindow(m_algorithmName);

    int initTrackbarValue = 10;
    setThresholdFromSlider(initTrackbarValue, &m_threshold);
    cv::createTrackbar("Threshold", m_algorithmName, &initTrackbarValue, 255, setThresholdFromSlider, &m_threshold);
}

///////////////////////////////////////////////////////////////////////////////
void SegmentMotionDiff::updateBackground(const cv::Mat& currentFrame)
{
    cv::cvtColor(currentFrame, m_background, CV_RGB2GRAY);
}

///////////////////////////////////////////////////////////////////////////////
void SegmentMotionDiff::setThresholdFromSlider(int thresholdSlider, void* paramsPtr)
{
    int* data = static_cast<int*>(paramsPtr);
    *data = thresholdSlider;
}

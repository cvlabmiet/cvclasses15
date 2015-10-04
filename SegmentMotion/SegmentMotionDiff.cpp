///@File: SegmentMotionDiff.cpp
///@Brief: Contains implementation of SegementMotionDiff class
///@Author: Vitaliy Baldeev
///@Date: 01 October 2015

#include "SegmentMotionDiff.h"

///////////////////////////////////////////////////////////////////////////////
void SegmentMotionDiff::run()
{
    cv::VideoCapture capture(0);

    if (capture.isOpened())
    {
        createGUI();
        getBackground(capture);

        while (true)
        {
            capture >> m_currentFrame;

            apply(m_currentFrame, m_foreground);
            cv::imshow("SegmentMotionDiff", m_foreground);

            if (cv::waitKey(1) >= 0)
            {
                break;
            }
        }
    }
    else
    {
        std::cerr << "Can not open the camera !" << std::endl;
        exit(-1);
    }
}

///////////////////////////////////////////////////////////////////////////////
void SegmentMotionDiff::createGUI()
{
    std::string winName = "SegmentMotionDiff";
    cv::namedWindow(winName);

    int initTrackbarValue = 10;
    setThresholdFromSlider(initTrackbarValue, &m_threshold);
    cv::createTrackbar("Threshold", winName, &initTrackbarValue, 255, setThresholdFromSlider, &m_threshold);
}

///////////////////////////////////////////////////////////////////////////////
void SegmentMotionDiff::getBackground(cv::VideoCapture& capture)
{
    capture >> m_background;
    cv::cvtColor(m_background, m_background, CV_RGB2GRAY);
}

///////////////////////////////////////////////////////////////////////////////
void SegmentMotionDiff::apply(const cv::Mat& currentFrame, cv::Mat& foreground)
{
    cv::Mat grayCurrentFrame;
    cv::cvtColor(currentFrame, grayCurrentFrame, CV_RGB2GRAY);

    foreground = abs(m_background - grayCurrentFrame);
    cv::threshold(foreground, foreground, m_threshold, 255, CV_THRESH_BINARY);
}

///////////////////////////////////////////////////////////////////////////////
void SegmentMotionDiff::setThresholdFromSlider(int thresholdSlider, void* paramsPtr)
{
    int* data = static_cast<int*>(paramsPtr);
    *data = thresholdSlider;
}

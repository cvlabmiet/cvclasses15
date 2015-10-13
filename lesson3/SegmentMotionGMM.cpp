///@File: SegmentMotionGMM.cpp
///@Brief: Contains implementation of segmentation based on gaussian mixture
///        model
///@Author: Vitaliy Baldeev
///@Date: 04 October 2015

#include "SegmentMotionGMM.h"

#include <iostream>

#include <opencv2\videoio\videoio.hpp>
#include "opencv2\highgui.hpp"

///////////////////////////////////////////////////////////////////////////////
cv::Mat SegmentMotionGMM::process(cv::VideoCapture& capture)
{
    if(!m_algorithmPtr)
    {
        m_algorithmPtr = cv::createBackgroundSubtractorMOG2();
    }

    cv::Mat currentFrame;
    capture >> currentFrame;

    cv::Mat result;
    m_algorithmPtr->apply(currentFrame, result);
    return result;
}

///////////////////////////////////////////////////////////////////////////////
void SegmentMotionGMM::createGUI()
{
    const std::string windowName = GetName();
    cv::namedWindow(windowName);

    int initValue = 30;

    setLearningrateFromSlider(initValue, &m_params);
    setHistoryFromSlider(initValue, &m_params);
    setVarThresholdFromSlider(initValue, &m_params);

    cv::createTrackbar("Learning Rate", windowName, &initValue, 100, setLearningrateFromSlider, &m_params);
    cv::createTrackbar("History", windowName, &initValue, 1000, setHistoryFromSlider, &m_params);
    cv::createTrackbar("Var Threshold", windowName, &initValue, 255, setVarThresholdFromSlider, &m_params);
}

///////////////////////////////////////////////////////////////////////////////
void SegmentMotionGMM::setLearningrateFromSlider(int learningRateSlider, void* paramsPtr)
{
  Params* data = static_cast<Params*>(paramsPtr);
  data->learningRate = learningRateSlider;
}

///////////////////////////////////////////////////////////////////////////////
void SegmentMotionGMM::setHistoryFromSlider(int historySlider, void* paramsPtr)
{
    Params* data = static_cast<Params*>(paramsPtr);
    data->history = historySlider;
}

///////////////////////////////////////////////////////////////////////////////
void SegmentMotionGMM::setVarThresholdFromSlider(int varThesholdSlider, void* paramsPtr)
{
    Params* data = static_cast<Params*>(paramsPtr);
    data->varThreshold = varThesholdSlider;
}

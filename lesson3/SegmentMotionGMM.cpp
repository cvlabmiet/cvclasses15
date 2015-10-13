///@File: SegmentMotionGMM.cpp
///@Brief: Contains implementation of segmentation based on gaussian mixture
///        model
///@Author: Vitaliy Baldeev
///@Date: 04 October 2015

#include <iostream>
#include "SegmentMotionGMM.h"
#include "opencv2\videoio\videoio.hpp"
#include "opencv2\highgui.hpp"

///////////////////////////////////////////////////////////////////////////////
std::string SegmentMotionGMM::GetName()
{
    return m_algorithmName;
}
///////////////////////////////////////////////////////////////////////////////
void SegmentMotionGMM::createGUI()
{
    cv::namedWindow(m_algorithmName);

    int initValue = 30;

    setLearningrateFromSlider(initValue, &m_params);
    setHistoryFromSlider(initValue, &m_params);
    setVarThresholdFromSlider(initValue, &m_params);

    cv::createTrackbar("Learning Rate", m_algorithmName, &initValue, 100, setLearningrateFromSlider, &m_params);
    cv::createTrackbar("History", m_algorithmName, &initValue, 1000, setHistoryFromSlider, &m_params);
    cv::createTrackbar("Var Threshold", m_algorithmName, &initValue, 255, setVarThresholdFromSlider, &m_params);
}

///////////////////////////////////////////////////////////////////////////////
void SegmentMotionGMM::apply(cv::Mat& currentFrame, cv::Mat& foreground)
{
    m_algorithmPtr->apply(currentFrame, foreground);
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

///@File: SegmentMotionBU.cpp
///@Brief: Contains implementation of SegementMotionBU class
///@Author: Vitaliy Baldeev
///@Date: 03 October 2015

#include "SegmentMotionBU.h"
#include <iostream>

///////////////////////////////////////////////////////////////////////////////
std::string SegmentMotionBU::GetName()
{
    return m_algorithmName;
}
///////////////////////////////////////////////////////////////////////////////
void SegmentMotionBU::apply(cv::Mat& currentFrame, cv::Mat& foreground)
{
    if (!m_prevBackgroundUpdated)
    {
        cv::cvtColor(currentFrame, m_prevBackground, CV_RGB2GRAY);
        m_prevBackgroundUpdated = true;
    }

    cvtColor(currentFrame, currentFrame, CV_RGB2GRAY);
    updateBackground(currentFrame);

    cv::Mat absDif = abs(m_background - currentFrame.clone());
    cv::threshold(absDif, absDif, m_params.threshold, 255, CV_THRESH_BINARY);
    foreground = absDif;
}

///////////////////////////////////////////////////////////////////////////////
void SegmentMotionBU::createGUI()
{
    cv::namedWindow(m_algorithmName);

    int initSliderValue = 10;

    setAlphaFromSlider(initSliderValue, &m_params);
    setThresholdFromSlider(initSliderValue, &m_params);

    cv::createTrackbar("Threshold", m_algorithmName, &initSliderValue, 255,
        setThresholdFromSlider, &m_params);

    cv::createTrackbar("Alpha", m_algorithmName, &initSliderValue, 100,
        setAlphaFromSlider, &m_params);
}

///////////////////////////////////////////////////////////////////////////////
void SegmentMotionBU::updateBackground(cv::Mat& currentFrame)
{
    float scaled_alpha = m_params.alpha * 0.01f;

    cv::Mat_<float> floatPrevBackground(m_prevBackground);
    cv::Mat_<float> floatCurrentFrame(currentFrame);
    cv::Mat_<float> floatBackground = (1 - scaled_alpha) * floatPrevBackground +
        scaled_alpha * floatCurrentFrame;

    floatBackground.convertTo(m_background, CV_8U);

    m_background.copyTo(m_prevBackground);
}

///////////////////////////////////////////////////////////////////////////////
void SegmentMotionBU::setThresholdFromSlider(int thresholdSlider, void* changeableDataPtr)
{
    Params* data = static_cast<Params*>(changeableDataPtr);
    data->threshold = thresholdSlider;
}

///////////////////////////////////////////////////////////////////////////////
void SegmentMotionBU::setAlphaFromSlider(int alphaSlider, void* changeableDataPtr)
{
    Params* data = static_cast<Params*>(changeableDataPtr);
    data->alpha = alphaSlider;
}

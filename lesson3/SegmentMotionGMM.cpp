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
void SegmentMotionGMM::Run()
{
    cv::VideoCapture capture(0);

    createGUI();

    if (capture.isOpened())
    {
        while (true)
        {
            capture >> m_currentFrame;
            m_algorithmPtr->apply(m_currentFrame, m_foreground, m_params.learningRate * 0.01);

            cv::imshow("SegmentMotionGMM", m_foreground);

            if (cv::waitKey(30) >= 0)
            {
                break;
            }
        }
    }
    else
    {
        std::cerr << " Can not open the camera !" << std::endl;
        exit(-1);
    }
}

///////////////////////////////////////////////////////////////////////////////
void SegmentMotionGMM::createGUI()
{
    std::string winName = "SegmentMotionGMM";

    cv::namedWindow(winName);

    int initValue = 30;

    setLearningrateFromSlider(initValue, &m_params);
    setHistoryFromSlider(initValue, &m_params);
    setVarThresholdFromSlider(initValue, &m_params);

    cv::createTrackbar("Learning Rate", winName, &initValue, 100, setLearningrateFromSlider, &m_params);
    cv::createTrackbar("History", winName, &initValue, 1000, setHistoryFromSlider, &m_params);
    cv::createTrackbar("Var Threshold", winName, &initValue, 255, setVarThresholdFromSlider, &m_params);
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

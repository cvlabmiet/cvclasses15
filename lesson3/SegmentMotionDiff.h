///@File: SegmentMotionDiff.h
///@Brief: Contains implementation of simple segmentation based on difference between two frames
///@Author: Vitaliy Baldeev
///@Date: 01 October 2015

#pragma once

#include <iostream>
#include "ISegmentMotion.h"
#include "SegmentMotionFactory.h"
#include "opencv2\videoio.hpp"
#include "opencv2\core\mat.hpp"
#include "opencv2\imgproc.hpp"
#include "opencv2\highgui.hpp"

///@class SegmentMotionDiff
/// Demonstrates the algorithm of simplest background subtraction with
/// no background updating
class SegmentMotionDiff : public ISegmentMotion
{
public:

    ///@brief Launch demostration
    void Run();

private:

    ///@brief Apply algorithm
    void apply(const cv::Mat& currentFrame, cv::Mat& foreground);

    ///@brief Create trackbar
    void createGUI();

    ///@brief Update background
    void getBackground(cv::VideoCapture& capture);

    ///@brief Set m_threshld from trackbar
    static void setThresholdFromSlider(int thresholdSlider, void* paramsPtr);

    ///@brief threshold of segmentation
    int m_threshold;

    cv::Mat m_background;       ///< background image
    cv::Mat m_currentFrame;     ///< current image from camera
    cv::Mat m_foreground;       ///< binary image with moving objects
};

namespace
{
    ///@brief Create the instance of SegmentMotionDiff
    ISegmentMotion* CreateSegmentMotionDiff()
    {
        return new SegmentMotionDiff;
    }

    const bool diffRegistered =
        SegmentMotionFactory::Instance().RegisterAlgorithm("Diff", CreateSegmentMotionDiff);  ///< registration in SegmentMotionFactory
}

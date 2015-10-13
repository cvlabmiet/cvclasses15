///@File: SegmentMotionDiff.h
///@Brief: Contains implementation of simple segmentation based on difference between two frames
///@Author: Vitaliy Baldeev
///@Date: 01 October 2015

#pragma once

#include <iostream>
#include "SegmentMotionBase.h"
#include "opencv2\videoio.hpp"
#include "opencv2\core\mat.hpp"
#include "opencv2\imgproc.hpp"
#include "opencv2\highgui.hpp"

///@class SegmentMotionDiff
/// Demonstrates the algorithm of simplest background subtraction with
/// no background updating
class SegmentMotionDiff : public SegmentMotionBase
{
public:

    ///@brief Get the name of algorithm
    std::string GetName();
private:

    ///@brief Apply algorithm
    void apply(cv::Mat& currentFrame, cv::Mat& foreground);

    ///@brief Create trackbar
    void createGUI();

    ///@brief Update background
    void updateBackground(const cv::Mat& currentFrame);

    ///@brief Set m_threshld from trackbar
    static void setThresholdFromSlider(int thresholdSlider, void* paramsPtr);

    ///@brief threshold of segmentation
    int m_threshold;

    bool m_backgroundUpdated = false;

    const std::string m_algorithmName = "Diff";
};

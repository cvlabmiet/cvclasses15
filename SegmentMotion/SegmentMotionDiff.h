///@File: SegmentMotionDiff.h
///@Brief: Contains implementation of simple segmentation based on difference between two frames
///@Author: Vitaliy Baldeev
///@Date: 01 October 2015

#pragma once

#include "opencv2\videoio.hpp"
#include "opencv2\core\mat.hpp"
#include "opencv2\imgproc.hpp"
#include "opencv2\highgui.hpp"
#include <iostream>

class SegmentMotionDiff
{
public:

    ///@brief Launch demostration
    void SegmentMotionDiff::run();

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

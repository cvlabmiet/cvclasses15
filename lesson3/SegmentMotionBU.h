///@File: SegmentMotionBU.h
///@Brief: Contains implementation of segmentation based on difference
///        between two frames and background updating
///@Author: Vitaliy Baldeev
///@Date: 03 October 2015

#pragma once

#include "SegmentMotionBase.h"
#include "opencv2\videoio.hpp"
#include "opencv2\core\mat.hpp"
#include "opencv2\imgproc.hpp"
#include "opencv2\highgui.hpp"

///@class SegmentMotionBU
/// Demonstrates the algorithm of simplest background subtraction with
/// background updating per every frame
class SegmentMotionBU : public SegmentMotionBase
{
public:

    ///@brief Get the name of algorithm
    std::string GetName();

private:
    ///@brief Apply algorithm
    void apply(cv::Mat& src, cv::Mat& dst);

    ///@brief Create trackbars
    void createGUI();

    ///@brief Update background
    void updateBackground(cv::Mat& currentFrame);

    ///@brief Set m_params.threshold from trackbar
    static void setThresholdFromSlider(int thresholdSlider, void* paramsPtr);

    ///@brief Set m_params.alpha from trackbar
    static void setAlphaFromSlider(int alphaSlider, void* paramsPtr);

    ///@brief structure of parameters with single object - m_params
    struct Params
    {
        int threshold;          ///< maximum of distance between background and current frame
        int alpha;              ///< velocity of learning
    } m_params;

    cv::Mat m_prevBackground;   ///< previous backround image

    bool m_prevBackgroundUpdated = false;

    const std::string m_algorithmName = "BU";
};

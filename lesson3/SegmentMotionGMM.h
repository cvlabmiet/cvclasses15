///@File: SegmentMotionGMM.h
///@Brief: Contains implementation of segmentation based on gaussian mixture
///        model
///@Author: Vitaliy Baldeev
///@Date: 04 October 2015

#pragma once

#include "SegmentMotionFactory.h"
#include "opencv2\core\mat.hpp"
#include "opencv2\video\background_segm.hpp"


///@class SegmentMotionGMM
/// Demonstrates the gaussian mixture algorithm of background subtraction
class SegmentMotionGMM : public ISegmentMotion
{
public:

    ///@brief Launch demostration
    void Run();

private:
    ///@brief Create trackbars
    void createGUI();

    ///@brief Set m_params.threshld from trackbar
    static void setLearningrateFromSlider(int learningRateSlider, void* paramsPtr);

    ///@brief Set m_params.history from trackbar
    static void setHistoryFromSlider(int historySlider, void* paramsPtr);

    ///@brief Set m_params.varThreshold from trackbar
    static void setVarThresholdFromSlider(int varThresholdSlider, void* paramsPtr);

    cv::Mat m_currentFrame;     ///< current image from camera
    cv::Mat m_foreground;       ///< binary image with moving objects

    ///@brief structure of parameters with single object - m_params
    struct Params
    {
        int learningRate;       ///< velocity of learning
        int history;            ///< how many frames take into account in computing of background
        int varThreshold;       ///< maximum of distance between background and current frame
    }m_params;

    ///@brief Pointer to OpenCV algorithm of background subtraction
    const cv::Ptr<cv::BackgroundSubtractorMOG2> m_algorithmPtr =
                                                cv::createBackgroundSubtractorMOG2();
};

namespace
{
    ISegmentMotion* CreateSegmentMotionGMM()
    {
        return new SegmentMotionGMM;
    }

    const bool gmmRegistered =
        SegmentMotionFactory::Instance().RegisterAlgorithm("GMM", CreateSegmentMotionGMM);
}

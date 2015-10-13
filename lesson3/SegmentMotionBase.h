///@File: ISegmentMotion.h
///@Brief: Contains interface for SegmentMotion classes
///@Author: Vitaliy Baldeev
///@Date: 10 October 2015

#pragma once

#include <string>
#include "opencv2\core\mat.hpp"
#include <memory>

///@class ISegmentMotion
/// Interface for SegmentMotion classes
class SegmentMotionBase
{
public:
    ///@brief Launch demonstration
    virtual void Run();
    
    ///@brief Destructor
    virtual ~SegmentMotionBase() {};
    
    ///@brief Get the name of algorithm 
    virtual std::string GetName() = 0;

    ///@brief Fabric method
    static SegmentMotionBase* CreateAlgorithm(std::string algorithmName);
protected:

    ///@brief Create trackbars
    virtual void createGUI() {};

    ///@brief Apply the algorthm of background subtraction
    virtual void apply(cv::Mat& currentFrame, cv::Mat& foreground) = 0;
    
    ///@brief Update current background frame
    virtual void updateBackground(cv::Mat& currentFrame) {};

protected:
    cv::Mat m_background;       ///< background image
    cv::Mat m_currentFrame;     ///< current image from camera
    cv::Mat m_foreground;       ///< binary image with moving objects
};

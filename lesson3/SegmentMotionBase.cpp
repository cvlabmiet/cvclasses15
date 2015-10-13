///@File: ISegmentMotion.cpp
///@Brief: Contains implementation of interface for SegmentMotion classes
///@Author: Vitaliy Baldeev
///@Date: 12 October 2015

#include "SegmentMotionBase.h"
#include "SegmentMotionDiff.h"
#include "SegmentMotionBU.h"
#include "SegmentMotionGMM.h"
#include "opencv2\videoio.hpp"
#include "opencv2\highgui.hpp"
#include <iostream>

///////////////////////////////////////////////////////////////////////////////
void SegmentMotionBase::Run()
{
    cv::VideoCapture capture(0);

    if (capture.isOpened())
    {
        createGUI();

        while (true)
        {
            capture >> m_currentFrame;

            apply(m_currentFrame, m_foreground);
            cv::imshow(this->GetName(), m_foreground);

            if (cv::waitKey(1) >= 0)
            {
                break;
            }
        }
    }
    else
    {
        std::cerr << "Can not open the camera !" << std::endl;
        exit(-1);
    }
}

///////////////////////////////////////////////////////////////////////////////
SegmentMotionBase* SegmentMotionBase::CreateAlgorithm(std::string& algorithmName)
{
    if (algorithmName == "Diff")
    {
        return new SegmentMotionDiff;
    }
    else if (algorithmName == "BU")
    {
        return new SegmentMotionBU;
    }
    else if (algorithmName == "GMM")
    {
        return new SegmentMotionGMM;
    }
    else
    {
        return nullptr;
    }
}

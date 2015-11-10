///@File: FeatureDetectorFAST.h
///@Brief: Contains declaration of FeatureDetectorFAST class
///@Author: Stepan Sidorov
///@Date: 01 November 2015

#pragma once

#include "stdafx.h"
#include "FeatureDetectorBase.h"

class FeatureDetectorFAST : public FeatureDetectorBase
{
public:
    ///@brief Constructor
    FeatureDetectorFAST();

    ///@see FeatureDetectorBase::Run
    virtual void Run(const cv::Mat &img);

    ///@see FeatureDetectorBase::GetName
    virtual std::string GetName() const override
    {
        return "FAST detector";
    }

protected:
    ///@brief find feature points
    static void findFeatures(int pos, void *data);

    struct Params
    {
        int seriesLength;       ///< Length of series (usually 12, but here we can change it)
        int threshold;          ///< Value of threshold

        cv::String windowName;  ///< Name of window
        cv::Mat srcImage;       ///< Image to be proccessed
    } m_param;
};

///@File: FeatureDetectorFAST.cpp
///@Brief: Contains implementation of FeatureDetectorFAST class
///@Author: Stepan Sidorov
///@Date: 01 November 2015

#pragma once

#include "stdafx.h"
#include "FeatureDetectorFAST.h"

FeatureDetectorFAST::FeatureDetectorFAST()
{
    m_param = { 12, 10 };
}

void FeatureDetectorFAST::Run(const cv::Mat &img)
{
    m_param.srcImage = img;

    // Create DEMO window
    m_param.windowName = GetName();
    cv::namedWindow(m_param.windowName, CV_WINDOW_AUTOSIZE);

    cv::createTrackbar("SLength",
        m_param.windowName, &m_param.seriesLength, 16, findFeatures, static_cast<void*>(&m_param));
    cv::createTrackbar("Thresh",
        m_param.windowName, &m_param.threshold, 100, findFeatures, static_cast<void*>(&m_param));

    cv::waitKey(0);
    cv::destroyWindow(m_param.windowName);
}

void FeatureDetectorFAST::findFeatures(int pos, void *data)
{
    const Params& userData = *static_cast<Params*>(data);

    cv::Mat gray, show;
    cv::cvtColor(userData.srcImage, gray, CV_BGR2GRAY);
    // 16 isn`t magic number (truly). It`s size of neighbourhood
    std::vector<int> neighbours(16 + userData.seriesLength - 1);
    userData.srcImage.copyTo(show);

    for (int i = 3; i < gray.rows - 3; i++)
    {
        for (int j = 3; j < gray.cols - 3; j++)
        {
            int val = static_cast<int>(gray.at<uchar>(i, j));

            neighbours[0] = static_cast<int>(gray.at<uchar>(i - 3, j + 0));
            neighbours[1] = static_cast<int>(gray.at<uchar>(i - 3, j + 1));
            neighbours[2] = static_cast<int>(gray.at<uchar>(i - 2, j + 2));
            neighbours[3] = static_cast<int>(gray.at<uchar>(i - 1, j + 3));

            neighbours[4] = static_cast<int>(gray.at<uchar>(i + 0, j + 3));
            neighbours[5] = static_cast<int>(gray.at<uchar>(i + 1, j + 3));
            neighbours[6] = static_cast<int>(gray.at<uchar>(i + 2, j + 2));
            neighbours[7] = static_cast<int>(gray.at<uchar>(i + 3, j + 1));

            neighbours[8] =  static_cast<int>(gray.at<uchar>(i + 3, j - 0));
            neighbours[9] =  static_cast<int>(gray.at<uchar>(i + 3, j - 1));
            neighbours[10] = static_cast<int>(gray.at<uchar>(i + 2, j - 2));
            neighbours[11] = static_cast<int>(gray.at<uchar>(i + 1, j - 3));

            neighbours[12] = static_cast<int>(gray.at<uchar>(i - 0, j - 3));
            neighbours[13] = static_cast<int>(gray.at<uchar>(i - 1, j - 3));
            neighbours[14] = static_cast<int>(gray.at<uchar>(i - 2, j - 2));
            neighbours[15] = static_cast<int>(gray.at<uchar>(i - 3, j - 1));

            // Duplicate first (userData.seriesLength - 1) pixels from the begining of vector because of easier computation
            for (int n = 16; n < neighbours.size(); n++)
            {
                neighbours[n] = neighbours[n - 16];
            }

            int t = userData.threshold;
            // Transform vector elements to 1, -1, 0
            std::for_each(neighbours.begin(), neighbours.end(), [val, t](int &elem)
            {
                if (elem - val > t)
                    elem = 1;
                if (val - elem > t)
                    elem = -1;
                else
                    elem = 0;
            });

            bool isFeature = false;
            int serLength = userData.seriesLength;
            // Sequentially check series of elements
            for (int n = 0; n < 16; n++)
            {
                int sum = 0;
                std::for_each(neighbours.begin() + n, neighbours.begin() + n + serLength, [&sum](int elem)
                {
                    sum += elem;
                });

                if (abs(sum) == serLength)
                {
                    isFeature = true;
                    break;
                }
            }

            if (isFeature)
            {
                cv::rectangle(show, cv::Point(j - 1, i - 1), cv::Point(j + 1, i + 1), cv::Scalar(255, 0, 255));
            }
        }
    }

    // Show result
    cv::imshow(userData.windowName, show);
}

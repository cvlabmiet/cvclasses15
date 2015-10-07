///@File: HarrisCornerDetector.cpp
///@Brief: Contains implementation of HarrisCornerDetector class
///@Author: Stepan Sidorov
///@Date: 06 October 2015

#include "stdafx.h"
#include "HarrisCornerDetector.h"

HarrisCornerDetector::HarrisCornerDetector()
{
   m_param = { 5, 5, 1, 1, "New window" };
}

void HarrisCornerDetector::Show(const cv::String& imgPath)
{
   // Load an image
   m_param.srcImage = cv::imread(imgPath);
   if (!m_param.srcImage.data)
   {
      return;
   }

   // Create window with original image
   cv::namedWindow(imgPath, CV_WINDOW_AUTOSIZE);
   cv::imshow(imgPath, m_param.srcImage);

   // Create DEMO window
   m_param.windowName = imgPath + " " + ReplyName();
   cv::namedWindow(m_param.windowName, CV_WINDOW_AUTOSIZE);

   cv::createTrackbar("WinSize",
      m_param.windowName, &m_param.windowSize, 7, detectFeatures, static_cast<void*>(&m_param));
   cv::createTrackbar("GSigma",
      m_param.windowName, &m_param.gaussianSigma, 15, detectFeatures, static_cast<void*>(&m_param));
   cv::createTrackbar("K*100",
      m_param.windowName, &m_param.parameterK, 15, detectFeatures, static_cast<void*>(&m_param));
   cv::createTrackbar("Thresh",
      m_param.windowName, &m_param.threshold, 25, detectFeatures, static_cast<void*>(&m_param));

   cv::waitKey(0);
}

void HarrisCornerDetector::detectFeatures(int pos, void* data)
{
   const Params& userData = *static_cast<Params*>(data);

   if (userData.windowSize % 2 == 0)
   {
      // skip invalid values
      return;
   }

   // Calculate weight matrix
   cv::Mat weightMatrix;
   if (userData.gaussianSigma == 0)
   {
      // Use weighted average window
      weightMatrix = cv::Mat::ones(userData.windowSize, userData.windowSize, CV_32FC1) / userData.windowSize / userData.windowSize;
   }
   else
   {
      // Use gaussian window
      cv::Mat gaussianKernel = cv::getGaussianKernel(userData.windowSize, userData.gaussianSigma, CV_32FC1);
      cv::mulTransposed(gaussianKernel, weightMatrix, false);
   }

   // Convert to grayscale
   cv::Mat gray;
   cvtColor(userData.srcImage, gray, CV_RGB2GRAY);

   // Remove noise by blurring with a Gaussian filter
   cv::Mat bluredImg;
   cv::GaussianBlur(gray, bluredImg, cv::Size(3, 3), 1);

   // Filtering blured image with Sobel masks
   cv::Mat IxxMat, IxyMat, IyyMat;
   cv::Sobel(bluredImg, IxxMat, CV_32FC1, 2, 0);
   cv::Sobel(bluredImg, IxyMat, CV_32FC1, 1, 1);
   cv::Sobel(bluredImg, IyyMat, CV_32FC1, 0, 2);

   cv::Mat result = cv::Mat::zeros(userData.srcImage.rows, userData.srcImage.cols, CV_32FC1);

   // Compute result matrix
   for (int i = (userData.windowSize - 1) / 2; i < userData.srcImage.rows - (userData.windowSize - 1) / 2; i++)
   {
      for (int j = (userData.windowSize - 1) / 2; j < userData.srcImage.cols - (userData.windowSize - 1) / 2; j++)
      {
         float Ixx = 0, Ixy = 0, Iyy = 0;
         for (int p = 0; p < userData.windowSize; p++)
         {
            for (int q = 0; q < userData.windowSize; q++)
            {
               int x = j - (userData.windowSize - 1) / 2 + q;
               int y = i - (userData.windowSize - 1) / 2 + p;
               Ixx += weightMatrix.at<float>(p, q) * IxxMat.at<float>(y, x);
               Ixy += weightMatrix.at<float>(p, q) * IxyMat.at<float>(y, x);
               Iyy += weightMatrix.at<float>(p, q) * IyyMat.at<float>(y, x);
            }
         }

         // Use original Harris corner measure
         result.at<float>(i, j) = (Ixx * Iyy - Ixy*Ixy) * static_cast<float>(userData.parameterK) / 100 * ((Ixx + Iyy) * (Ixx + Iyy));
      }
   }

   cv::Mat dst = userData.srcImage.clone();
   // Threshold filtering and extracting local maxima
   for (int i = 1; i < userData.srcImage.rows - 1; i++)
   {
      for (int j = 1; j < userData.srcImage.cols - 1; j++)
      {
         // Threshold filtering
         if (result.at<float>(i, j) < static_cast<float>(userData.threshold))
         {
            continue;
         }

         // Extract local maxima
         bool isFeature = true;
         for (int p = i - 1; i < p + 1; i++)
         {
            for (int q = j - 1; j < q + 1; j++)
            {
               if (result.at<float>(p, q) > result.at<float>(i, j))
               {
                  isFeature = false;
                  break;
               }
            }
         }
         if (isFeature)
         {
            cv::circle(dst, cv::Point(j, i), 2, cv::Scalar(255, 255, 0));
         }
      }
   }

   // Show result
   cv::imshow(userData.windowName, dst);
}

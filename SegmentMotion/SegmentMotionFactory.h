///@File: SegmentMotionFactory.h
///@Brief: Contains implementation factory for SegmentMotion classes
///@Author: Vitaliy Baldeev
///@Date: 10 October 2015

#pragma once

#include "ISegmentMotion.h"
#include <map>
#include <string>

///@class SegmentMotionFactory
/// Factory class for creating SegmentMotion classes
class SegmentMotionFactory
{
public:
  typedef ISegmentMotion* (*CreateSegmentMotion)();

private:
    typedef std::map<std::string, CreateSegmentMotion> smmap;

    SegmentMotionFactory() {}
    SegmentMotionFactory(const SegmentMotionFactory&);
    SegmentMotionFactory(const SegmentMotionFactory&&);
    SegmentMotionFactory& operator=(const SegmentMotionFactory&);
    SegmentMotionFactory& operator=(const SegmentMotionFactory&&);

    smmap m_algorithmsMap;

public:
  static SegmentMotionFactory& Instance()
  {
    static SegmentMotionFactory factory;
    return factory;
  }

  bool RegisterAlgorithm(const std::string& algorithmId, CreateSegmentMotion creator)
  {
    return m_algorithmsMap.insert(smmap::value_type(algorithmId, creator)).second;
  }

  bool UnregiterAlgorithm(const std::string& algorithmId)
  {
    return m_algorithmsMap.erase(algorithmId) == 1;
  }

  ISegmentMotion* CreateAlgorithm(std::string& algorithmId)
  {
    smmap::const_iterator result = m_algorithmsMap.find(algorithmId);

    if (result != m_algorithmsMap.end())
    {
      return (result->second)();
    }
    else
    {
        return nullptr;
    }
  }
};

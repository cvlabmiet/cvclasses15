#include <iostream>
#include <memory>

#include "SegmentMotionDiff.h"
#include "SegmentMotionBU.h"
#include "SegmentMotionGMM.h"

int main()
{
    std::cout << "Select the algorithm: \n"
              << "Diff  - Basic difference \n"
              << "BU    - Basic difference with background updating \n"
              << "GMM   - Gaussian mixture model algorithm \n";

    std::string algorithmName;
    std::cin >> algorithmName;

    std::unique_ptr<ISegmentMotion>
        ptr(SegmentMotionFactory::Instance().CreateAlgorithm(algorithmName));

    if (ptr)
    {
        ptr->Run();
    }
    else
    {
        std::cout << algorithmName<< " algorithm is not created!" << std::endl;
    }
    return 0;
}

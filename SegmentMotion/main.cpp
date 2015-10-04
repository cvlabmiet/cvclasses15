#include "SegmentMotionDiff.h"
#include "SegmentMotionBU.h"
#include "SegmentMotionGMM.h"

#include <iostream>

int main()
{
    int algorithmNumber = 0;

    std::cout << "Select the algorithm: \n"
              << "1 - Basic difference \n"
              << "2 - Basic difference with background updating \n"
              << "3 - Gaussian mixture model algorithm \n";

    std::cin >> algorithmNumber;

    if (1 == algorithmNumber)
    {
        SegmentMotionDiff smd;
        smd.run();
    }
    else if (2 == algorithmNumber)
    {
        SegmentMotionBU smbu;
        smbu.run();
    }
    else if (3 == algorithmNumber)
    {
        SegmentMotionGMM smgmm;
        smgmm.run();
    }
    else
    {
        std::cout << "Algorithm is not chosen." << std::endl;
    }

    std::cout << "Press any key for exit..." << std::endl;

    return 0;
}

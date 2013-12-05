/* Start Header -------------------------------------------------------
Copyright (C) 2013 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: CoreTime.h
Purpose: Header file for a time class for the engine
Language: C++
Platform: Windows
Project: CS529_twalton_FinalProject
Author: Tommy Walton, t.walton, 130000812
Creation date: 10/3/2013
- End Header -----------------------------------------------------*/
#pragma once

#include "Precomp.h"

#define MILLESECONDS_PER_SECOND 1000.0f

namespace Framework
{
    class CoreTime
    {
    public:
        CoreTime();
        ~CoreTime(){};

        unsigned int GetTimeInMilliseconds(void);
        float GetFrameTimeInMilliseconds(void);
        
    private:
        LARGE_INTEGER TicksPerSecond;
        LARGE_INTEGER PerformanceCounterStart;
        LARGE_INTEGER PerformanceCounterLast;
        LARGE_INTEGER PerformanceCounterCurrent;
    };
}

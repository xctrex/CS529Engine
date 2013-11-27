/* Start Header -------------------------------------------------------
Copyright (C) 2013 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: CoreTime.cpp
Purpose: Implementation for a time class for the engine
Language: C++
Platform: Windows
Project: CS529_twalton_FinalProject
Author: Tommy Walton, t.walton, 130000812
Creation date: 10/3/2013
- End Header -----------------------------------------------------*/
#include "CoreTime.h"

namespace Framework
{
    CoreTime::CoreTime()
    {
        // Get the frequency
        ThrowErrorIf(!QueryPerformanceFrequency(&TicksPerSecond), "The installed hardware does not support a high frequency timer"); // TODO: create a fall back plan if this fails
        // Set the start time, previous time, and current time
        QueryPerformanceCounter(&PerformanceCounterStart);
        PerformanceCounterLast = PerformanceCounterCurrent = PerformanceCounterStart;
    }
    
    unsigned int CoreTime::GetTimeInMilliseconds()
    {
        // Get the current time
        QueryPerformanceCounter(&PerformanceCounterCurrent);
        // Return the difference between the current time and the start time
        return (unsigned int)((PerformanceCounterCurrent.QuadPart - PerformanceCounterStart.QuadPart) / (TicksPerSecond.QuadPart)) * MILLESECONDS_PER_SECOND;
    }

    unsigned int CoreTime::GetFrameTimeInMilliseconds()
    {
        // Get the current time
        QueryPerformanceCounter(&PerformanceCounterCurrent);
        // Calculate the difference between the current time and the previous frame
        unsigned int dt = (unsigned int)((PerformanceCounterCurrent.QuadPart - PerformanceCounterLast.QuadPart) / (TicksPerSecond.QuadPart)) * MILLESECONDS_PER_SECOND;
        // Set the new last frame to the current time
        PerformanceCounterLast = PerformanceCounterCurrent;
        // Return the difference between the last frame and the current time
        return dt;
    }
}

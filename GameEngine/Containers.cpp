/* Start Header -------------------------------------------------------
Copyright (C) 2013 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Containers.cpp
Purpose: Implementation file for containers
Language: C++
Platform: Windows
Project: CS529_twalton_FinalProject
Author: Tommy Walton, t.walton, 130000812
Creation date: 12/15/2013
- End Header -----------------------------------------------------*/
#include "Containers.h"


int cstrtoi(char* cstr, int length)
{
    int i, digits, total = 0;
    // Count the number of digits
    for (digits = 0; digits < length; ++digits)
    {
        if (cstr[digits] == '\n') break;
    }
    // Add up each digit
    for (i = 0; i < digits; ++i)
    {
        int temp = cstr[i] - '0';
        total += (cstr[i] - '0') * (int)pow(10.0f, (digits - i - 1));
    }
    return total;
}

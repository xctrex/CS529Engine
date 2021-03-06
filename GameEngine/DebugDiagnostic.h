/* Start Header -------------------------------------------------------
Copyright (C) 2013 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: DebugDiagnostics.h
Purpose: Header for error/assertion and debug printing
Language: C++
Platform: Windows
Project: CS529_twalton_FinalProject
Author: Tommy Walton, t.walton, 130000812
Creation date: 10/3/2013
- End Header -----------------------------------------------------*/
#pragma once

#define VERIFY_SUCCEEDED(hr) return hr == S_OK;

#include <stdio.h>

// If the condition is true, print a message
void ThrowErrorIf(bool, char *);


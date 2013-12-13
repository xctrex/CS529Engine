/* Start Header -------------------------------------------------------
Copyright (C) 2013 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Matrix2D.c
Purpose: This file contains the implementation for a 2D matrix library
Language: C
Platform: Windows
Project: CS529_twalton_Project3_Part1
Author: Tommy Walton, t.walton, 130000812
Creation date: 9/16/2013
- End Header -----------------------------------------------------*/

#include "Matrix2D.h"

#ifndef PI
#define PI 3.14159265f
#endif

/*
This function sets the matrix Result to the identity matrix
*/
void Matrix2DIdentity(Matrix2D *pResult)
{
    pResult->m[0][0] = 1.0f;
    pResult->m[0][1] = 0.0f;
    pResult->m[0][2] = 0.0f;

    pResult->m[1][0] = 0.0f;
    pResult->m[1][1] = 1.0f;
    pResult->m[1][2] = 0.0f;

    pResult->m[2][0] = 0.0f;
    pResult->m[2][1] = 0.0f;
    pResult->m[2][2] = 1.0f;
}

// ---------------------------------------------------------------------------

/*
This functions calculated the transpose matrix of Mtx and saves it in Result
*/
void Matrix2DTranspose(Matrix2D *pResult, Matrix2D *pMtx)
{
    //Only create the temporary variable "tempMtx" if pResult == pMtx
    if(pResult == pMtx)
    {
        Matrix2D tempMtx;
        int i;
        for(i = 0; i < 3; ++i)
        {
            int j;
            for(j = 0; j < 3; ++j)
            {
                tempMtx.m[i][j] = pMtx->m[j][i];
            }
        }
        for(i = 0; i < 3; ++i)
        {
            int j;
            for(j = 0; j < 3; ++j)
            {
                pResult->m[i][j] = tempMtx.m[i][j];
            }
        }
    }
    else
    {
        int i;
        for(i = 0; i < 3; ++i)
        {
            int j;
            for(j = 0; j < 3; ++j)
            {
                pResult->m[i][j] = pMtx->m[j][i];
            }
        }
    }
}

// ---------------------------------------------------------------------------

/*
This function multiplies Mtx0 with Mtx1 and saves the result in Result
Result = Mtx0*Mtx1
*/
void Matrix2DConcat(Matrix2D *pResult, Matrix2D *pMtx0, Matrix2D *pMtx1)
{
    if(pResult == pMtx0 || pResult == pMtx1)
    {
        Matrix2D tempMtx;
        
        /*tempMtx->m[0][0] = pMtx0->m[0][0] * pMtx1->m[0][0] + pMtx0->m[0][1] * pMtx1->m[1][0] + pMtx0->m[0][2] * pMtx1->m[2][0];
        tempMtx->m[0][1] = pMtx0->m[0][0] * pMtx1->m[0][1] + pMtx0->m[0][1] * pMtx1->m[1][1] + pMtx0->m[0][2] * pMtx1->m[2][1];
        tempMtx->m[0][2] = pMtx0->m[0][0] * pMtx1->m[0][2] + pMtx0->m[0][1] * pMtx1->m[1][2] + pMtx0->m[0][2] * pMtx1->m[2][2];

        tempMtx->m[1][0] = pMtx0->m[1][0] * pMtx1->m[0][0] + pMtx0->m[1][1] * pMtx1->m[1][0] + pMtx0->m[1][2] * pMtx1->m[2][0];
        tempMtx->m[1][1] = pMtx0->m[1][0] * pMtx1->m[0][1] + pMtx0->m[1][1] * pMtx1->m[1][1] + pMtx0->m[1][2] * pMtx1->m[2][1];
        tempMtx->m[1][2] = pMtx0->m[1][0] * pMtx1->m[0][2] + pMtx0->m[1][1] * pMtx1->m[1][2] + pMtx0->m[1][2] * pMtx1->m[2][2];
        
        tempMtx->m[2][0] = pMtx0->m[2][0] * pMtx1->m[0][0] + pMtx0->m[2][1] * pMtx1->m[1][0] + pMtx0->m[2][2] * pMtx1->m[2][0];
        tempMtx->m[2][1] = pMtx0->m[2][0] * pMtx1->m[0][1] + pMtx0->m[2][1] * pMtx1->m[1][1] + pMtx0->m[2][2] * pMtx1->m[2][1];
        tempMtx->m[2][2] = pMtx0->m[2][0] * pMtx1->m[0][2] + pMtx0->m[2][1] * pMtx1->m[1][2] + pMtx0->m[2][2] * pMtx1->m[2][2];*/
        int i;
        for(i = 0; i < 3; ++i)
        {
            int j;
            for(j = 0; j < 3; ++j)
            {
                int k;
                tempMtx.m[i][j] = 0.0f;
                for(k = 0; k < 3; ++k)
                {
                    tempMtx.m[i][j] += pMtx0->m[i][k] * pMtx1->m[k][j];
                }
            }
        }
        for(i = 0; i < 3; ++i)
        {
            int j;
            for(j = 0; j < 3; ++j)
            {
                pResult->m[i][j] = tempMtx.m[i][j];
            }
        }
    }
    else
    {
        /*pResult->m[0][0] = pMtx0->m[0][0] * pMtx1->m[0][0] + pMtx0->m[0][1] * pMtx1->m[1][0] + pMtx0->m[0][2] * pMtx1->m[2][0];
        pResult->m[0][1] = pMtx0->m[0][0] * pMtx1->m[0][1] + pMtx0->m[0][1] * pMtx1->m[1][1] + pMtx0->m[0][2] * pMtx1->m[2][1];
        pResult->m[0][2] = pMtx0->m[0][0] * pMtx1->m[0][2] + pMtx0->m[0][1] * pMtx1->m[1][2] + pMtx0->m[0][2] * pMtx1->m[2][2];

        pResult->m[1][0] = pMtx0->m[1][0] * pMtx1->m[0][0] + pMtx0->m[1][1] * pMtx1->m[1][0] + pMtx0->m[1][2] * pMtx1->m[2][0];
        pResult->m[1][1] = pMtx0->m[1][0] * pMtx1->m[0][1] + pMtx0->m[1][1] * pMtx1->m[1][1] + pMtx0->m[1][2] * pMtx1->m[2][1];
        pResult->m[1][2] = pMtx0->m[1][0] * pMtx1->m[0][2] + pMtx0->m[1][1] * pMtx1->m[1][2] + pMtx0->m[1][2] * pMtx1->m[2][2];
        
        pResult->m[2][0] = pMtx0->m[2][0] * pMtx1->m[0][0] + pMtx0->m[2][1] * pMtx1->m[1][0] + pMtx0->m[2][2] * pMtx1->m[2][0];
        pResult->m[2][1] = pMtx0->m[2][0] * pMtx1->m[0][1] + pMtx0->m[2][1] * pMtx1->m[1][1] + pMtx0->m[2][2] * pMtx1->m[2][1];
        pResult->m[2][2] = pMtx0->m[2][0] * pMtx1->m[0][2] + pMtx0->m[2][1] * pMtx1->m[1][2] + pMtx0->m[2][2] * pMtx1->m[2][2];*/
        int i;
        for(i = 0; i < 3; ++i)
        {
            int j;
            for(j = 0; j < 3; ++j)
            {
                int k;
                pResult->m[i][j] = 0.0f;
                for(k = 0; k < 3; ++k)
                {
                    pResult->m[i][j] += pMtx0->m[i][k] * pMtx1->m[k][j];
                }
            }
        }
    }
}

// ---------------------------------------------------------------------------

/*
This function creates a translation matrix from x *p y and saves it in Result
*/
void Matrix2DTranslate(Matrix2D *pResult, float x, float y)
{
    //|1|0|x|
    //|0|1|y|
    //|0|0|1|
    Matrix2DIdentity(pResult);
    pResult->m[0][2] = x;
    pResult->m[1][2] = y;
}

// ---------------------------------------------------------------------------

/*
This function creates a scaling matrix from x *p y and saves it in Result
*/
void Matrix2DScale(Matrix2D *pResult, float x, float y)
{
    //|x|0|0|
    //|0|y|0|
    //|0|0|1|
    Matrix2DIdentity(pResult);
    pResult->m[0][0] = x;
    pResult->m[1][1] = y;
}

// ---------------------------------------------------------------------------

/*
This matrix creates a rotation matrix from "Angle" whose value is in degree.
Save the resultant matrix in Result
*/
void Matrix2DRotDeg(Matrix2D *pResult, float Angle)
{
    Matrix2DRotRad(pResult, Angle * PI / 180.0f);
}

// ---------------------------------------------------------------------------

/*
This matrix creates a rotation matrix from "Angle" whose value is in radian.
Save the resultant matrix in Result
*/
void Matrix2DRotRad(Matrix2D *pResult, float Angle)
{
    //|cos(angle)|-sin(angle)|0|
    //|sin(angle)| cos(angle)|0|
    //|    0     |     0     |1|
    Matrix2DIdentity(pResult);
    pResult->m[0][0] = pResult->m[1][1] = cos(Angle);
    pResult->m[1][0] = sin(Angle);
    pResult->m[0][1] = pResult->m[1][0] * -1.0f;//-sin(Angle)
}

// ---------------------------------------------------------------------------

/*
This function multiplies the matrix Mtx with the vector Vec and saves the result in Result
Result = Mtx * Vec
*/
void Matrix2DMultVec(Vector2D *pResult, Matrix2D *pMtx, Vector2D *pVec)
{
    if(pResult == pVec)
    {
        Vector2D tmpVec;
        tmpVec.x = pMtx->m[0][0] * pVec->x + pMtx->m[0][1] * pVec->y + pMtx->m[0][2];
        tmpVec.y = pMtx->m[1][0] * pVec->x + pMtx->m[1][1] * pVec->y + pMtx->m[1][2];
        pResult->x = tmpVec.x;
        pResult->y = tmpVec.y;
    }
    else
    {
        pResult->x = pMtx->m[0][0] * pVec->x + pMtx->m[0][1] * pVec->y + pMtx->m[0][2];
        pResult->y = pMtx->m[1][0] * pVec->x + pMtx->m[1][1] * pVec->y + pMtx->m[1][2];
    }
}

// ---------------------------------------------------------------------------

#ifndef __FUN_H__
#define __FUN_H__

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

void printMtx(float* mtx);
float* mtxMulMxt(float* mtx1, float* mtx2);
void mtxMulVec(float* mtx, float* vec);
float* getScaleMtx(float unitsX, float unitsY, float unitsZ);
void scale(float* vec, float units);
float* getRotateZMtx(float angle);
void rotateZ(float* vec, float angle);
float* getRotateYMtx(float angle);
void rotateY(float* vec, float angle);
float* getRotateXMtx(float angle);
void rotateX(float* vec, float angle);
float* getTranslationMtx(float unitsX, float unitsY, float unitsZ);
void translationRight(float* vec, float units);
void translationLeft(float* vec, float units);
void translationUp(float* vec, float units);
void translationDown(float* vec, float units);

static float identily[] = {
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f,
};

static float mirrorYZ[] = {
    -1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f,
};

static float mirrorXZ[] = {
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, -1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f,
};

#endif
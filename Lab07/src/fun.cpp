
#include <fun/fun.h>

void mtxMulVec(float* mtx, float* vec)
{
    float* tempVec = new float[4];
    for (int i = 0; i < 3; i++)
    {
        tempVec[i] = vec[i];
    }
    tempVec[3] = 1;

    float* temp = new float[4];
    int k = 0;
    for (int i = 0; i < 4; i++)
    {
        temp[i] = 0;
        for (int j = 0; j < 4; j++)
        {
            temp[i] += mtx[k] * tempVec[j];
            k++;
        }
    }
    for (int i = 0; i < 3; i++)
    {
        vec[i] = temp[i];
    }
}

void scale(float* vec, float s)
{
    identily[0] = s;
    identily[5] = s;
    identily[10] = s;

    mtxMulVec(identily, vec);
    mtxMulVec(identily, vec + 3);
    mtxMulVec(identily, vec + 6);

    identily[0] = 1;
    identily[5] = 1;
    identily[10] = 1;
}

void rotateZ(float* vec, float angle)
{
    float mtx[] = {
        cos(angle), -sin(angle), 0.0f, 0.0f,
        sin(angle), cos(angle), 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    };

    mtxMulVec(mtx, vec);
    mtxMulVec(mtx, vec + 3);
    mtxMulVec(mtx, vec + 6);
}

void rotateY(float* vec, float angle)
{
    float mtx[] = {
        cos(angle), 0.0f, sin(angle), 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        -sin(angle), 0.0f, cos(angle), 0.0f,         
        0.0f, 0.0f, 0.0f, 1.0f,
    };

    mtxMulVec(mtx, vec);
    mtxMulVec(mtx, vec + 3);
    mtxMulVec(mtx, vec + 6);
}

void rotateX(float* vec, float angle)
{
    float mtx[] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, cos(angle), -sin(angle), 0.0f,
        0.0f, sin(angle), cos(angle), 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    };

    mtxMulVec(mtx, vec);
    mtxMulVec(mtx, vec + 3);
    mtxMulVec(mtx, vec + 6);
}
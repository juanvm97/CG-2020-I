
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

    identily[0] = 1;
    identily[5] = 1;
    identily[10] = 1;
}

void rotateZ(float* vec, float angle)
{
    
    float mtx[] = {
        cos(M_PI * angle / 180), -sin(M_PI * angle / 180), 0.0f, 0.0f,
        sin(M_PI * angle / 180), cos(M_PI * angle / 180), 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    };

    mtxMulVec(mtx, vec);
}

void rotateY(float* vec, float angle)
{
    float mtx[] = {
        cos(M_PI * angle / 180), 0.0f, sin(M_PI * angle / 180), 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        -sin(M_PI * angle / 180), 0.0f, cos(M_PI * angle / 180), 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    };

    mtxMulVec(mtx, vec);
}

void rotateX(float* vec, float angle)
{
    float mtx[] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, cos(M_PI * angle / 180), -sin(M_PI * angle / 180), 0.0f,
        0.0f, sin(M_PI * angle / 180), cos(M_PI * angle / 180), 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    };

    mtxMulVec(mtx, vec);
}
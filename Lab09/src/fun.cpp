
#include <fun/fun.h>

void printMtx(float* mtx)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            std::cout << mtx[(i * 4) + j] << " ";
        }
        std::cout << std::endl;
    }
}

float* mtxMulMxt(float* mtx1, float* mtx2)
{
    float* result = new float[16];

    float temp = 0.0f;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            temp = 0.0f;
            for (int k = 0; k < 4; k++)
            {
                temp += (mtx1[(i * 4) + k] * mtx2[(k * 4) + j]);
            }
            result[(i * 4) + j] = temp;
        }        
    }

    return result;
}

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

float* getScaleMtx(float unitsX, float unitsY, float unitsZ)
{
    float* result = new float[16];

    for (int i = 0; i < 16; i++)
    {
        result[i] = 0.0f;
    }
    result[0] = unitsX;
    result[5] = unitsY;
    result[10] = unitsZ;
    result[15] = 1.0f;

    return result;
}

void scale(float* vec, float units)
{
    float* mtx = getScaleMtx(units, units, units);

    mtxMulVec(mtx, vec);
}

float* getRotateZMtx(float angle)
{
    float temp[] = {
        cos(M_PI * angle / 180), -sin(M_PI * angle / 180), 0.0f, 0.0f,
        sin(M_PI * angle / 180), cos(M_PI * angle / 180), 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    };
    
    float* result = new float[16];
    for (int i = 0; i < 16; i++)
    {
        result[i] = temp[i];
    }

    return result;
}

void rotateZ(float* vec, float angle)
{    
    float* mtx = getRotateZMtx(angle);

    mtxMulVec(mtx, vec);
}

float* getRotateYMtx(float angle)
{
    float temp[] = {
        cos(M_PI * angle / 180), 0.0f, sin(M_PI * angle / 180), 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        -sin(M_PI * angle / 180), 0.0f, cos(M_PI * angle / 180), 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    };

    float* result = new float[16];
    for (int i = 0; i < 16; i++)
    {
        result[i] = temp[i];
    }

    return result;
}

void rotateY(float* vec, float angle)
{
    float* mtx = getRotateYMtx(angle);

    mtxMulVec(mtx, vec);
}

float* getRotateXMtx(float angle)
{
    float temp[] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, cos(M_PI * angle / 180), -sin(M_PI * angle / 180), 0.0f,
        0.0f, sin(M_PI * angle / 180), cos(M_PI * angle / 180), 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    };

    float* result = new float[16];
    for (int i = 0; i < 16; i++)
    {
        result[i] = temp[i];
    }

    return result;
}

void rotateX(float* vec, float angle)
{
    float* mtx = getRotateXMtx(angle);

    mtxMulVec(mtx, vec);
}

float* getTranslationMtx(float unitsX, float unitsY, float unitsZ)
{
    float* result = new float[16];
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (i == j)
            {
                result[(i * 4) + j] = 1.0f;
            }
            else
            {
                result[(i * 4) + j] = 0.0f;
            }            
        }
    }
    result[3] = unitsX;
    result[7] = unitsY;
    result[11] = unitsZ;

    return result;
}

void translationRight(float* vec, float units)
{
    float* mtx = getTranslationMtx(units, 0.0f, 0.0f);

    mtxMulVec(mtx, vec);
}

void translationLeft(float* vec, float units)
{
    float* mtx = getTranslationMtx(-units, 0.0f, 0.0f);

    mtxMulVec(mtx, vec);
}

void translationUp(float* vec, float units)
{
    float* mtx = getTranslationMtx(0.0f, units, 0.0f);

    mtxMulVec(mtx, vec);
}

void translationDown(float* vec, float units)
{
    float* mtx = getTranslationMtx(0.0f, -units, 0.0f);

    mtxMulVec(mtx, vec);
}

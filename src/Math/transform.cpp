#include "transform.h"
#include "constants.h"
#include <math.h>

Vec2f Normalize(const Vec2f& vec)
{
    return vec / vec.Length();
}

Vec3f Normalize(const Vec3f& vec)
{
    return vec / vec.Length();
}

Vec4f Normalize(const Vec4f& vec)
{
    return vec / vec.Length();
}

float Radians(float degrees)
{
    return degrees * (float)(PXL_PI / 180.0);
}

Mat4f Translate(const Mat4f& mat, const Vec3f& tVec)
{
    Mat4f result = mat;
    result[3] = result[0] * tVec.x + result[1] * tVec.y + result[2] * tVec.z + result[3];
    return result;
}

// TODO: Probably wrong!
Mat4f RotateX(Mat4f& mat, float angle)
{
    float s = sin(angle);
    float c = cos(angle);

    Mat4f RotX(1.0f);
    RotX[1].y = c;
    RotX[1].z = s;
    RotX[2].y = -s;
    RotX[2].z = c;
    return RotX * mat;
}

Mat4f RotateY(Mat4f& mat, float angle)
{
    float s = sin(angle);
    float c = cos(angle);

    Mat4f RotY(1.0f);
    RotY[0].x = c;
    RotY[0].z = -s;
    RotY[2].x = s;
    RotY[2].z = c;
    return RotY * mat;
}

Mat4f RotateZ(Mat4f& mat, float angle)
{
    float s = sin(angle);
    float c = cos(angle);

    Mat4f RotZ(1.0f);
    RotZ[0].x = c;
    RotZ[0].y = s;
    RotZ[1].x = -s;
    RotZ[1].y = c;
    return RotZ * mat;
}

Mat4f Scale(Mat4f& mat, const Vec3f& sVec)
{
    Mat4f result = mat;
    result[0] = result[0] * sVec.x;
    result[1] = result[1] * sVec.y;
    result[2] = result[2] * sVec.z;
    return result;
}

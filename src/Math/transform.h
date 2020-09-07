#pragma once
#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "mat4.h"

Vec2f Normalize(const Vec2f& vec);
Vec3f Normalize(const Vec3f& vec);
Vec4f Normalize(const Vec4f& vec);

float Radians(float degrees);

Mat4f Translate(const Mat4f& mat, const Vec3f& tVec);
Mat4f RotateX(Mat4f& mat, float angle);
Mat4f RotateY(Mat4f& mat, float angle);
Mat4f RotateZ(Mat4f& mat, float angle);
Mat4f Scale(Mat4f& mat, const Vec3f& sVec);
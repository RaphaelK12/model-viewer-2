#pragma once
#include "vec4.h"

struct Mat4f
{
	Vec4f val[4];

	// Identity matrix
	Mat4f(float x);
	Mat4f(const Vec4f& r1, const Vec4f& r2, const Vec4f& r3, const Vec4f& r4);
	Vec4f& operator[](int index);
};

Mat4f operator*(Mat4f& lhs, Mat4f& rhs);
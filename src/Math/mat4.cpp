#include "mat4.h"
#include <cstdio>

Mat4f::Mat4f(float x)
{
	val[0].y = val[0].z = val[0].w = 0.0f;
	val[1].x = val[1].z = val[1].w = 0.0f;
	val[2].x = val[2].y = val[2].w = 0.0f;
	val[3].x = val[3].y = val[3].z = 0.0f;

	val[0].x = val[1].y = val[2].z = val[3].w = x;
}

Mat4f::Mat4f(const Vec4f& r1, const Vec4f& r2, const Vec4f& r3, const Vec4f& r4)
{
	val[0] = r1;
	val[1] = r2;
	val[2] = r3;
	val[3] = r4;
}

Vec4f& Mat4f::operator[](int index)
{
	if(index < 0 || index > 3)
	{
		printf("Index of Mat4 out of bounds: %d\n", index);
		return val[0];
	}
	return val[index];
}

// Column major
Mat4f operator*(Mat4f& lhs, Mat4f& rhs)
{
	Vec4f r1;
	r1.x = lhs[0].x * rhs[0].x + lhs[0].y * rhs[1].x + lhs[0].z * rhs[2].x + lhs[0].w * rhs[3].x;
	r1.y = lhs[0].x * rhs[0].y + lhs[0].y * rhs[1].y + lhs[0].z * rhs[2].y + lhs[0].w * rhs[3].y;
	r1.z = lhs[0].x * rhs[0].z + lhs[0].y * rhs[1].z + lhs[0].z * rhs[2].z + lhs[0].w * rhs[3].z;
	r1.w = lhs[0].x * rhs[0].w + lhs[0].y * rhs[1].w + lhs[0].z * rhs[2].w + lhs[0].w * rhs[3].w;

	Vec4f r2;
	r2.x = lhs[1].x * rhs[0].x + lhs[1].y * rhs[1].x + lhs[1].z * rhs[2].x + lhs[1].w * rhs[3].x;
	r2.y = lhs[1].x * rhs[0].y + lhs[1].y * rhs[1].y + lhs[1].z * rhs[2].y + lhs[1].w * rhs[3].y;
	r2.z = lhs[1].x * rhs[0].z + lhs[1].y * rhs[1].z + lhs[1].z * rhs[2].z + lhs[1].w * rhs[3].z;
	r2.w = lhs[1].x * rhs[0].w + lhs[1].y * rhs[1].w + lhs[1].z * rhs[2].w + lhs[1].w * rhs[3].w;

	Vec4f r3;
	r3.x = lhs[2].x * rhs[0].x + lhs[2].y * rhs[1].x + lhs[2].z * rhs[2].x + lhs[2].w * rhs[3].x;
	r3.y = lhs[2].x * rhs[0].y + lhs[2].y * rhs[1].y + lhs[2].z * rhs[2].y + lhs[2].w * rhs[3].y;
	r3.z = lhs[2].x * rhs[0].z + lhs[2].y * rhs[1].z + lhs[2].z * rhs[2].z + lhs[2].w * rhs[3].z;
	r3.w = lhs[2].x * rhs[0].w + lhs[2].y * rhs[1].w + lhs[2].z * rhs[2].w + lhs[2].w * rhs[3].w;

	Vec4f r4;
	r4.x = lhs[3].x * rhs[0].x + lhs[3].y * rhs[1].x + lhs[3].z * rhs[2].x + lhs[3].w * rhs[3].x;
	r4.y = lhs[3].x * rhs[0].y + lhs[3].y * rhs[1].y + lhs[3].z * rhs[2].y + lhs[3].w * rhs[3].y;
	r4.z = lhs[3].x * rhs[0].z + lhs[3].y * rhs[1].z + lhs[3].z * rhs[2].z + lhs[3].w * rhs[3].z;
	r4.w = lhs[3].x * rhs[0].w + lhs[3].y * rhs[1].w + lhs[3].z * rhs[2].w + lhs[3].w * rhs[3].w;

	return Mat4f(r1, r2, r3, r4);
}

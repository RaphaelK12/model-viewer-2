#include "vec4.h"
#include <math.h>

Vec4f::Vec4f(float v)
{
	x = y = z = w = v;
}

Vec4f::Vec4f(float _x, float _y, float _z, float _w)
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}

float Vec4f::Length() const
{
	return sqrtf(x * x + y * y + z * z + w * w);
}

Vec4f operator+(const Vec4f& lhs, const Vec4f& rhs)
{
	return Vec4f(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
}

Vec4f operator*(const Vec4f& lhs, float rhs)
{
	return Vec4f(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs);
}

Vec4f operator/(const Vec4f& lhs, float rhs)
{
	return Vec4f(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs);
}

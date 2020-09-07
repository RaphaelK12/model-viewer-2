#include "vec3.h"
#include <math.h>

Vec3f::Vec3f(float v)
{
	x = y = z = v;
}

Vec3f::Vec3f(float _x, float _y, float _z)
{
	x = _x;
	y = _y;
	z = _z;
}

float Vec3f::Length() const
{
	return sqrtf(x * x + y * y + z * z);
}

Vec3f operator+(const Vec3f& lhs, const Vec3f& rhs)
{
	return Vec3f(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

Vec3f operator-(const Vec3f& lhs, const Vec3f& rhs)
{
	return Vec3f(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}

Vec3f operator-(const Vec3f& rhs)
{
	return Vec3f(-rhs.x, -rhs.y, -rhs.z);
}

Vec3f operator*(const Vec3f& lhs, float rhs)
{
	return Vec3f(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
}

Vec3f operator/(const Vec3f& lhs, float rhs)
{
	return Vec3f(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs);
}

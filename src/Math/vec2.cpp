#include "vec2.h"
#include <math.h>

Vec2f operator+(const Vec2f& lhs, const Vec2f& rhs)
{
	return Vec2f(lhs.x + rhs.x, lhs.y + rhs.y);
}

Vec2f operator-(const Vec2f& lhs, const Vec2f& rhs)
{
	return Vec2f(lhs.x - rhs.x, lhs.y - rhs.y);
}

Vec2f operator/(const Vec2f& lhs, float rhs)
{
	return Vec2f(lhs.x / rhs, lhs.y / rhs);
}

Vec2f::Vec2f(float v)
{
	x = y = v;
}

Vec2f::Vec2f(float _x, float _y)
{
	x = _x;
	y = _y;
}

float Vec2f::Length() const
{
	return sqrtf(x * x + y * y);
}

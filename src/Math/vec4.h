#pragma once

struct Vec4f
{
	union
	{
		float x, y, z, w;
	};

	Vec4f() = default;
	Vec4f(float v);
	Vec4f(float _x, float _y, float _z, float _w);

	float Length() const;
};

Vec4f operator+(const Vec4f& lhs, const Vec4f& rhs);
Vec4f operator*(const Vec4f& lhs, float rhs);
Vec4f operator/(const Vec4f& lhs, float rhs);
#pragma once

struct Vec2f
{
	union
	{
		float x, y;
	};

	Vec2f() = default;
	Vec2f(float v);
	Vec2f(float _x, float _y);

	float Length() const;
};

Vec2f operator+(const Vec2f& lhs, const Vec2f& rhs);
Vec2f operator-(const Vec2f& lhs, const Vec2f& rhs);
Vec2f operator/(const Vec2f& lhs, float rhs);
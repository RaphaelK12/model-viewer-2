#pragma once

struct Vec3f
{
	union
	{
		float x, y, z;
	};

	Vec3f() = default;
	Vec3f(float v);
	Vec3f(float x, float y, float z);

	float Length() const;
};

Vec3f operator+(const Vec3f& lhs, const Vec3f& rhs);
Vec3f operator-(const Vec3f& lhs, const Vec3f& rhs);
Vec3f operator-(const Vec3f& rhs);
Vec3f operator*(const Vec3f& lhs, float rhs);
Vec3f operator/(const Vec3f& lhs, float rhs);
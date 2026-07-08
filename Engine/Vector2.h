#pragma once
#include <cmath>

namespace nu {
	struct Vector2 {
		float x, y;

		Vector2() = default;
		Vector2(float x, float y) : x{ x }, y{ y } {}
		Vector2(float xy) : x{ xy }, y{ xy } {}

		Vector2 operator + (const Vector2& v) const { return Vector2(this->x + v.x, this->y + v.y); }
		Vector2 operator - (const Vector2& v) const { return Vector2(this->x - v.x, this->y - v.y); }
		Vector2 operator * (const Vector2& v) const { return Vector2(this->x * v.x, this->y * v.y); }
		Vector2 operator / (const Vector2& v) const { return Vector2(this->x / v.x, this->y / v.y); }
	
		Vector2 operator + (float v) const { return Vector2(this->x + v, this->y + v); }
		Vector2 operator - (float v) const { return Vector2(this->x - v, this->y - v); }
		Vector2 operator * (float v) const { return Vector2(this->x * v, this->y * v); }
		Vector2 operator / (float v) const { return Vector2(this->x / v, this->y / v); }

		Vector2& operator += (const Vector2& v) { this->x += v.x; this->y += v.y; return *this; }
		Vector2& operator -= (const Vector2& v) { this->x -= v.x; this->y - v.y; return *this; }
		Vector2& operator *= (const Vector2& v) { this->x *= v.x; this->y * v.y; return *this; }
		Vector2& operator /= (const Vector2& v) { this->x /= v.x; this->y / v.y; return *this; }

		Vector2& operator += (float v) { this->x += v; this->y + v; return *this; }
		Vector2& operator -= (float v) { this->x -= v; this->y - v; return *this; }
		Vector2& operator *= (float v) { this->x *= v; this->y * v; return *this; }
		Vector2& operator /= (float v) { this->x /= v; this->y / v; return *this; }

		float LengthSqr() const { return (x * x) + (y * y); }
		float Length() const { return sqrt(LengthSqr()); }
	};
}
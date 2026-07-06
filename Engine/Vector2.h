#pragma once

namespace nu {
	struct Vector2 {
		float x, y;

		Vector2() = default;
		Vector2(float x, float y) : x{ x }, y{ y } {}
		Vector2(float xy) : x{ xy }, y{ xy } {}

		Vector2 operator + (Vector2 v) {
			return Vector2(this->x + v.x, this->y + v.y);
		}
	};
}
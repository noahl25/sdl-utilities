#pragma once

#include <iostream>

struct Vector2D {

	float x, y;

	Vector2D() : x(0), y(0) {}
	Vector2D(const float& x, const float& y): x(x), y(y) {}
	Vector2D(const Vector2D& other): x(other.x), y(other.y) {}

	inline int* asList() {
		return reinterpret_cast<int*>(&x);
	}

	inline void operator+=(const Vector2D& other) {

		x += other.x;
		y += other.y;

	}

	inline void operator-=(const Vector2D& other) {

		x -= other.x;
		y -= other.y;

	}

};
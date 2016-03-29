#pragma once
#include "Vector2D.h"

class Rectangle {
public:
	Vector2D center;
	float width, height;

	Rectangle();
	Rectangle(Vector2D, float, float);
	Rectangle(Vector2D v1, Vector2D v2);
	void set(Vector2D v1, Vector2D v2);
	bool isIn(Vector2D v);
};
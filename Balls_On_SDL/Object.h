#pragma once
#include "Vector2D.h"

struct RGB {
	int r, g, b;

	RGB() : r(0), g(0), b(0) {}
	RGB(int r, int g, int b) : r(r), g(g), b(b) {}

	RGB randomize() {
		r = rand() % 255;
		g = rand() % 255;
		b = rand() % 255;
		return RGB(r, g, b);
	}
};

class Object {
public:
	Vector2D position;
	Vector2D velocity;
	Vector2D acceleration;

	RGB color;
	float m;
};

#pragma once
#include "Vector2D.h"
#include "SDL.h"

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

	virtual void draw(SDL_Renderer *renderer) = 0;
	virtual void move(Vector2D delta) = 0;
	virtual void update(float delta_t) = 0;

	RGB color;
};

#pragma once
#include "Vector2D.h"
#include "Object.h"

class Rectangle : public Object {
public:
	float width, height;

	SDL_Rect rect;

	Rectangle();
	Rectangle(Vector2D center, float width, float height);
	Rectangle(Vector2D v1, Vector2D v2);

	void set(Vector2D v1, Vector2D v2);
	void set(Vector2D center, float width, float height);
	void draw(SDL_Renderer *renderer);
	bool isIn(Vector2D v);
};
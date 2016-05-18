#pragma once
#include "Vector2D.h"
#include "Object.h"

class Rectangle : public Object {
public:
	SDL_Rect rect;

	Rectangle();
	Rectangle(Vector2D center, float width, float height);
	Rectangle(Vector2D v1, Vector2D v2);

	virtual void move(Vector2D delta);
	void set(Vector2D v1, Vector2D v2);
	void set(Vector2D center, float width, float height);
	virtual void draw(SDL_Renderer *renderer);
	virtual void update(float delta_t);
	bool isIn(Vector2D v);
};
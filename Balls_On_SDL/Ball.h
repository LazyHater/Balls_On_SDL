#define _USE_MATH_DEFINES
#pragma once
#include "Object.h"
#include "Vector2D.h"
#include <math.h>
#include "graphics.h"


class Ball : public Object {
public:
	static int n;

	int r;
	float m;
	bool collided = false;

	float bounce_factor;

	Ball();
	Ball(const Ball  &ball);
	Ball(float x, float y, RGB c);
	Ball::~Ball();

	virtual void move(Vector2D delta);
	virtual void update(float delta_t);
	void draw(SDL_Renderer *renderer);
};
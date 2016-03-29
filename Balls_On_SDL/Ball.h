#define _USE_MATH_DEFINES
#pragma once
#include "Object.h"
#include "Vector2D.h"
#include <math.h>


class Ball : public Object {
public:
	static int n;

	int r = 5;
	bool collided = false;
	float bounce_factor = 0.9f;

	Ball();
	Ball(const Ball  &ball);
	Ball(float x, float y, RGB c);
	Ball::~Ball();

	void update();
};
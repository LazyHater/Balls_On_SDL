#define _USE_MATH_DEFINES
#pragma once
#include "Vector2D.h"
#include "graphics.h"
#include <math.h>


class Ball {
public:
	static int n;
	Vector2D position;
	Vector2D velocity;
	Vector2D acceleration;

	RGB color;

	int r = 5;
	float m;
	bool collided = false;
	float bounce_factor = 0.9f;

	Ball();
	Ball(const Ball  &ball);
	Ball(float x, float y, RGB c);
	Ball::~Ball();

	void update();
};
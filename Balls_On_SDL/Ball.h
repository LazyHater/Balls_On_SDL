#pragma once
#include "Vector2D.h"
#include "graphics.h"

class Ball
{
public:
	static int n;
	Vector2D position;
	Vector2D velocity;
	Vector2D acceleration;

	RGB color;

	float m = 1.0f;
	int r = 5;
	bool collided = false;
	float bounce_factor = 0.9f;

	Ball();
	Ball(const Ball  &ball);
	Ball(float x, float y, RGB c);
	Ball::~Ball();
	

	void update();

};
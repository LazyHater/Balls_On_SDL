#pragma once
#include <cmath>
#include <vector>
#include "Ball.h"
#include "Vector2D.h"

class BallSpawner
{
public:

	std::vector<Ball> balls;
	Vector2D position;

	bool ball_to_ball_collision = false;
	bool ball_to_ball_bounce_factor = false;
	bool gravity = false;
	int radius_of_balls = 5;
	float mass_of_balls = 1.0f;
	float bounce_factor = 0.9f;
	void handleCollisionBallToBall();
	void plowBalls(Ball &ball_1, Ball &ball_2, float distance);
	void deployBalls(Vector2D v, int n);
	void update();
	void apply();

};


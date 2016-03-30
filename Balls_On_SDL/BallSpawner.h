#pragma once
#include "Ball.h"
#include "Vector2D.h"
#include "graphics.h"
#include <cmath>
#include <vector>

class BallSpawner {
public:
	std::vector<Ball> balls;
	Vector2D position;
	Vector2D gravity_vector = Vector2D(0,150); 

	bool ball_to_ball_collision = false;
	bool ball_to_ball_bounce = false;
	bool gravity = false;
	bool random_radius = false;
	int radius_of_balls = 5;
	float bounce_factor = 0.9f;

	void handleCollisionBallToBall();
	void plowBalls(Ball &ball_1, Ball &ball_2, float distance);
	void ballsCollision(Ball &ball_1, Ball &ball_2);
	void deployBalls(Vector2D v, int n);
	void update(float delta_t);
};


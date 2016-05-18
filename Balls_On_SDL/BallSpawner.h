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

	bool ball_to_ball_collision = false;
	bool ball_to_ball_bounce = false;

	static int def_radius_of_balls;
	static float def_bounce_factor;
	static int def_balls_per_deploy;
	static float def_mass;

	void handleCollisionBallToBall();
	void plowBalls(Ball &ball_1, Ball &ball_2, float distance);
	void ballsCollision(Ball &ball_1, Ball &ball_2);
	void deployBalls(Vector2D v, int n = def_balls_per_deploy, 
					bool random_velocity = true, bool random_radius = false, float m = def_mass);
	void update(float delta_t);
};

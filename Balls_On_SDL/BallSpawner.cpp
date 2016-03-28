#include "BallSpawner.h"
void BallSpawner::plowBalls(Ball &ball_1, Ball &ball_2, float distance)
{
	Vector2D delta;
	float how_far = (ball_1.r + ball_2.r - distance) / 2.0f;

	if (ball_2.position == ball_1.position)
	{
		delta.setTryg(1.0, randFromTo(0, M_PI * 2));
	}
	else
	{
		delta = ball_2.position - ball_1.position;
		delta.normalize();
	}

	delta *= how_far;
	ball_1.position -= delta;
	ball_2.position += delta;
}

void BallSpawner::handleCollisionBallToBall() 
{
	for (std::vector<Ball>::iterator it = balls.begin(); it != balls.end(); ++it)
	{
		for (std::vector<Ball>::iterator jt = it + 1; jt != balls.end(); ++jt)
		{
			float distance = it->position.dist(jt->position);

			if (distance < it->r + jt->r)
			{
				plowBalls(*it, *jt, distance);
				it->velocity.swap(jt->velocity);
				if (ball_to_ball_bounce_factor)
				{
					it->collided = true;
					jt->collided = true;
				}
			}
		}
	}
}

void BallSpawner::deployBalls(Vector2D v, int n)
{
	balls.reserve(n);
	std::vector<Ball> temp(n);
	for (Ball ball : temp)
	{
		ball.velocity.setTryg(randFromTo(0.1, 4), randTo(M_PI * 2));
		ball.position = v;
		if(gravity)
			ball.acceleration = Vector2D(0, 0.1);
		ball.color = RGB(randFromTo(20, 255), 0, randFromTo(80, 255));
		ball.r = radius_of_balls;
		ball.m = mass_of_balls;
		ball.bounce_factor = bounce_factor;
		balls.push_back(ball);
	}
}

void BallSpawner::update()
{
	if (ball_to_ball_collision)
		handleCollisionBallToBall();
	for (Ball &ball : balls)
	{
		ball.update();
	}
}

void BallSpawner::apply()
{
	for (Ball &ball : balls)
	{
		if (gravity)
			ball.acceleration = Vector2D(0, 0.1);
		ball.color = RGB(randFromTo(20, 255), 0, randFromTo(80, 255));
		ball.r = radius_of_balls;
		ball.m = mass_of_balls;
		ball.bounce_factor = bounce_factor;
	}
}
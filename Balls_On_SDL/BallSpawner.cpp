#include "BallSpawner.h"

void BallSpawner::plowBalls(Ball &ball_1, Ball &ball_2, float distance) {
	Vector2D delta;
	float how_far = (ball_1.r + ball_2.r - distance) / 2.0f;

	if (ball_2.position == ball_1.position)	{
		delta.setTryg(1.0, randFromTo(0, M_PI * 2));
	}
	else {
		delta = ball_2.position - ball_1.position;
		delta.normalize();
	}

	delta *= how_far;
	ball_1.position -= delta;
	ball_2.position += delta;
}

void BallSpawner::handleCollisionBallToBall() {
	for (std::vector<Ball>::iterator it = balls.begin(); it != balls.end(); ++it) {
		for (std::vector<Ball>::iterator jt = it + 1; jt != balls.end(); ++jt) {
			float distance = it->position.dist(jt->position);
			if (distance < it->r + jt->r) {
				plowBalls(*it, *jt, distance);
				ballsCollision(*it, *jt);
			}		
		}
	}
}

void BallSpawner::ballsCollision(Ball &ball_1, Ball &ball_2) {
	if (ball_1.m == ball_2.m) {
		ball_1.velocity.swap(ball_2.velocity);
	}
	else {
		float mpm = ball_1.m + ball_2.m;
		Vector2D v1(ball_1.velocity);
		Vector2D v2(ball_2.velocity);

		ball_1.velocity = (v1*(ball_1.m - ball_2.m) + v2*ball_2.m*2.0f) / mpm; //((m1-m2)*v1+2m2v2)/(m1+m2)
		ball_2.velocity = (v2*(ball_2.m - ball_1.m) + v1*ball_1.m*2.0f) / mpm; //((m2-m1)*v2+2m1v1)/(m1+m2)
	}

	if (ball_to_ball_bounce) {
		ball_1.collided = true;
		ball_2.collided = true;
	}
}

void BallSpawner::deployBalls(Vector2D v, int n) {
	balls.reserve(n);
	std::vector<Ball> temp(n);
	for (Ball ball : temp) {
		ball.velocity.setTryg(randFromTo(20, 200), randTo(M_PI * 2));
		ball.position = v;
		ball.color = RGB(randFromTo(20, 255), 0, randFromTo(80, 255));
		ball.r = radius_of_balls;
		if(random_radius)
			ball.r = randFromTo(3, 15);
		ball.m = ball.r*ball.r*M_PI;
		ball.bounce_factor = bounce_factor;
		balls.push_back(ball);
	}
}

void BallSpawner::update(float delta_t) {
	if (ball_to_ball_collision)
		handleCollisionBallToBall();

	for (Ball &ball : balls) {
		ball.update(delta_t);
		if(gravity)
			ball.velocity += gravity_vector*delta_t;
	}
}

void BallSpawner::apply() {
	for (Ball &ball : balls) {
		if (gravity)
			ball.acceleration = Vector2D(0, 0.1);
		ball.color = RGB(randFromTo(20, 255), 0, randFromTo(80, 255));
		ball.r = radius_of_balls;
		ball.m = radius_of_balls*radius_of_balls*M_PI;
		ball.bounce_factor = bounce_factor;
	}
}
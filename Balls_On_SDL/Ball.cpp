#include "Ball.h"

int Ball::n = 0;

Ball::Ball() { n++; }

Ball::Ball(const Ball  &ball) : r(ball.r), bounce_factor(ball.bounce_factor) {
	color = ball.color;
	position = ball.position;
	velocity = ball.velocity;
	acceleration = ball.acceleration;
	m = r*r*M_PI;
	n++;
}

Ball::Ball(float x, float y, RGB c):  r(5) {
	color = c;
	position.set(x, y);
	m = r*r*M_PI;
	n++;
}

void Ball::update() {
	if (collided) {
		velocity = velocity*bounce_factor;
		collided = !collided;
	}
	velocity.add(acceleration);
	position.add(velocity);	
}


Ball::~Ball() {
	n--;
}
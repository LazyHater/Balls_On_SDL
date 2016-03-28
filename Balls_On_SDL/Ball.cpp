#include "Ball.h"

int Ball::n = 0;

Ball::Ball() { n++; }

Ball::Ball(const Ball  &ball) : r(ball.r), m(ball.m), bounce_factor(ball.bounce_factor) {
	color = ball.color;
	position = ball.position;
	velocity = ball.velocity;
	acceleration = ball.acceleration;
	n++;
}

Ball::Ball(float x, float y, RGB c) : color(c), r(5) {
	position.set(x, y);
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
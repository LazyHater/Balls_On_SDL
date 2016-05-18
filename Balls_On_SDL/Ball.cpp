#include "Ball.h"

int Ball::n = 0;

Ball::Ball() { n++; }

Ball::Ball(const Ball  &ball) : r(ball.r), bounce_factor(ball.bounce_factor) {
	color = ball.color;
	position = ball.position;
	velocity = ball.velocity;
	acceleration = ball.acceleration;
	m = ball.m;
	n++;
}

Ball::Ball(float x, float y, RGB c):  r(5) {
	color = c;
	position.set(x, y);
	m = r*r*M_PI;
	n++;
}

void Ball::move(Vector2D delta) {
	position += delta;
}

void Ball::update(float delta_t) {
	if (collided) {
		velocity = velocity*bounce_factor;
		collided = false;
	}
	velocity.add(acceleration*delta_t);
	position.add(velocity*delta_t);
}

void Ball::draw(SDL_Renderer * renderer) {
	Graphic::renderCircle(renderer, position, r, color);
}


Ball::~Ball() {
	n--;
}
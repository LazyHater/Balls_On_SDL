#include "Environment.h"

float Environment::speed_of_simulation = 1.0f;
bool Environment::gravity_forces = true;
int Environment::precision_of_calcs = 1;
Vector2D Environment::gravity_vector;

Environment::Environment(Rectangle box) : box(box) {
	this->box.color = RGB(255,0,0);
}

void Environment::handleCollisionWithScreen(std::vector<Ball>& balls) {
	for (Ball &ball : balls) {
		if (ball.position.x + ball.r > box.rect.x + box.rect.w) {
			ball.velocity.x = -ball.velocity.x;
			ball.position.x = box.rect.x + box.rect.w - ball.r;
			ball.collided = true;
		}
		if (ball.position.x - ball.r < box.rect.x) {
			ball.velocity.x = -ball.velocity.x;
			ball.position.x = box.rect.x + ball.r;
			ball.collided = true;
		}
		if (ball.position.y + ball.r > box.rect.y + box.rect.h) {
			ball.velocity.y = -ball.velocity.y;
			ball.position.y = box.rect.y + box.rect.h - ball.r;
			ball.collided = true;
		}
		if (ball.position.y - ball.r < box.rect.y) {
			ball.velocity.y = -ball.velocity.y;
			ball.position.y = box.rect.y + ball.r;
			ball.collided = true;
		}
	}
}

void Environment::handleCollisionWithLines(std::vector<Ball>& balls, std::vector<Line> lines) {
	for (Line line : lines) {
		for (Ball &ball : balls) {
			if (line.collideBox.isIn(ball.position)) {
				float distance = line.distance(ball.position) - line.width;
				if (distance < ball.r) {
					ball.velocity = ball.velocity - line.norVec*
						2.0*(ball.velocity*line.norVec); //R=V-2N(N*V)
					ball.collided = true;
					if (ball.velocity.angle(line.norVec) < M_PI / 2)
						ball.position += line.norVec*(ball.r - distance);
					else
						ball.position -= line.norVec*(ball.r - distance);
				}
			}
		}
	}
}

void Environment::handleCollisionWithRectangles(std::vector<Ball>& balls, std::vector<Rectangle>& rectangles) {
	for (Rectangle &rectangle : rectangles) {
		for (Ball &ball : balls) {
			if ((abs(rectangle.position.x - ball.position.x)<ball.r + rectangle.rect.w / 2.0f)
				&& (abs(rectangle.position.y - ball.position.y)<ball.r + rectangle.rect.h / 2.0f)) {

				if (abs(ball.position.x - rectangle.position.x + rectangle.rect.w / 2.0f) < ball.r) {
					ball.velocity.x = -ball.velocity.x*ball.bounce_factor;
					ball.position.x = rectangle.position.x - rectangle.rect.w / 2.0f - ball.r;
				}

				if (abs(ball.position.x - rectangle.position.x - rectangle.rect.w / 2.0f) < ball.r) {
					ball.velocity.x = -ball.velocity.x*ball.bounce_factor;
					ball.position.x = rectangle.position.x + rectangle.rect.w / 2.0f + ball.r;
				}

				if (abs(ball.position.y - rectangle.position.y - rectangle.rect.h / 2.0f) < ball.r) {
					ball.velocity.y = -ball.velocity.y*ball.bounce_factor;
					ball.position.y = rectangle.position.y + rectangle.rect.h / 2.0f + ball.r;
				}

				if (abs(ball.position.y - rectangle.position.y + rectangle.rect.h / 2.0f) < ball.r) {
					ball.velocity.y = -ball.velocity.y*ball.bounce_factor;
					ball.position.y = rectangle.position.y - rectangle.rect.h / 2.0f - ball.r;
				}
			}
		}
	}
}
#define P(x) std::cout<<#x<<": "<<x<<" "

void Environment::handleGravityForces(std::vector<Ball>& balls) {
	float G = 100;
	float force_max = 100000;

	for (Ball &ball : balls)  ball.acceleration = Vector2D(0, 0); 
	
	int temp = balls.size();
	for (int i = 0; i < temp - 1; i++) {
		for (int j = i + 1; j < temp; j++) {
			float r = Vector2D::distance(balls[i].position, balls[j].position);
			if (r <= balls[i].r + balls[j].r) r = balls[i].r + balls[j].r;

			float force = G*(balls[i].m*balls[j].m) / (r*r); //G*(m1*m2)/r^2
			if (force > force_max) force = force_max;

			Vector2D dir_vec = balls[j].position - balls[i].position; // vector from first ball to second
			Vector2D force_vec = dir_vec;
			force_vec.normalize();
			force_vec *= force;
			
			balls[i].acceleration += force_vec / balls[i].m;
			balls[j].acceleration += !force_vec / balls[j].m;
		}
	}

}

void Environment::update(float delta_t) {
	delta_t *= speed_of_simulation;

	for (int i = 0; i < precision_of_calcs; i++) {
		if (gravity_forces)
			handleGravityForces(BSpwn.balls);
		handleCollisionWithScreen(BSpwn.balls);
		handleCollisionWithLines(BSpwn.balls, lines);
		handleCollisionWithRectangles(BSpwn.balls, rectangles);
		BSpwn.update(delta_t / precision_of_calcs);
		for (Ball &ball : BSpwn.balls) {
			ball.velocity += gravity_vector*delta_t;
		}
	}
}

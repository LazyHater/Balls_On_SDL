#include "Environment.h"

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

void Environment::update(float delta_t) {
	delta_t *= speed_of_simulation;

	for (int i = 0; i < precision_of_calcs; i++) {
		handleCollisionWithScreen(BSpwn.balls);
		handleCollisionWithLines(BSpwn.balls, lines);
		handleCollisionWithRectangles(BSpwn.balls, rectangles);
		BSpwn.update(delta_t / precision_of_calcs);
		for (Ball &ball : BSpwn.balls) {
			ball.velocity += gravity_vector*delta_t;
		}
	}
}

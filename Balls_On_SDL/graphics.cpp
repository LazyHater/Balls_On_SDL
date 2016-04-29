#include "graphics.h"

float randTo(float to) {
	return static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / to));
}

float randFromTo(float from, float to)
{
	return from + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (to - from)));
}

std::vector<SDL_Point> Graphic::templateOfCircle;
int Graphic::radius_of_circle_template = 5;

void Graphic::renderCircle(SDL_Renderer *renderer, Vector2D position, int r, RGB color) {
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
	if (r == radius_of_circle_template) {
		drawCircleFromTemplate(renderer, position);
	}
	else {
		drawFilledCircle(renderer, position.x, position.y, r);
	}
}

void Graphic::drawCircleFromTemplate(SDL_Renderer * renderer, Vector2D position) {
	std::vector<SDL_Point> temp = templateOfCircle;
	for (int i = 0; i < templateOfCircle.size(); i++) {
		temp[i].x += position.x;
		temp[i].y += position.y;
	}
	SDL_RenderDrawPoints(renderer, temp.data(), temp.size());
}

void Graphic::makeTemplateOfCircle(int r) {
	templateOfCircle.clear();
	radius_of_circle_template = r;
	SDL_Point p;
	int x = r;
	int y = 0;
	int decisionOver2 = 1 - x;   // Decision criterion divided by 2 evaluated at x=r, y=0

	while (y <= x) {
		for (int i = -x; i <= x; i++) {
			p.x = i;
			p.y = y;
			templateOfCircle.push_back(p);
			p.y = -y;
			templateOfCircle.push_back(p);
		}

		for (int i = -y; i <= y; i++) {
			p.x = i;
			p.y = x;
			templateOfCircle.push_back(p);
			p.y = -x;
			templateOfCircle.push_back(p);
		}
		y++;

		if (decisionOver2 <= 0) {
			decisionOver2 += 2 * y + 1;   // Change in decision criterion for y -> y+1
		}
		else {
			x--;
			decisionOver2 += 2 * (y - x) + 1;   // Change for y -> y+1, x -> x-1
		}
	}
}

void Graphic::drawCircle(SDL_Renderer * renderer, int x0, int y0, int radius) {
int x = radius;
int y = 0;
int decisionOver2 = 1 - x;   // Decision criterion divided by 2 evaluated at x=r, y=0

while (y <= x) {
	SDL_RenderDrawPoint(renderer, x + x0, y + y0); // Octant 1
	SDL_RenderDrawPoint(renderer, y + x0, x + y0); // Octant 2
	SDL_RenderDrawPoint(renderer, -y + x0, x + y0); // Octant 3
	SDL_RenderDrawPoint(renderer, -x + x0, y + y0); // Octant 4
	SDL_RenderDrawPoint(renderer, -x + x0, -y + y0); // Octant 5
	SDL_RenderDrawPoint(renderer, -y + x0, -x + y0); // Octant 6
	SDL_RenderDrawPoint(renderer, x + x0, -y + y0); // Octant 7
	SDL_RenderDrawPoint(renderer, y + x0, -x + y0); // Octant 8
	y++;
	if (decisionOver2 <= 0) {
		decisionOver2 += 2 * y + 1;   // Change in decision criterion for y -> y+1
	}
	else {
		x--;
		decisionOver2 += 2 * (y - x) + 1;   // Change for y -> y+1, x -> x-1
	}
}
}

void Graphic::drawFilledCircle(SDL_Renderer * renderer, int x0, int y0, int radius) {
	int x = radius;
	int y = 0;
	int decisionOver2 = 1 - x;   // Decision criterion divided by 2 evaluated at x=r, y=0

	while (y <= x) {
		SDL_RenderDrawLine(renderer, x + x0, y + y0, -x + x0, y + y0);
		SDL_RenderDrawLine(renderer, y + x0, x + y0, -y + x0, x + y0);
		SDL_RenderDrawLine(renderer, -x + x0, -y + y0, x + x0, -y + y0);
		SDL_RenderDrawLine(renderer, y + x0, -x + y0, -y + x0, -x + y0);
		y++;

		if (decisionOver2 <= 0) {
			decisionOver2 += 2 * y + 1;   // Change in decision criterion for y -> y+1
		}
		else {
			x--;
			decisionOver2 += 2 * (y - x) + 1;   // Change for y -> y+1, x -> x-1
		}
	}
}

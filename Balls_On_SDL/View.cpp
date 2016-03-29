#include "View.h"

using namespace std;

View::View(int width, int height, bool full_screen){
	srand(time(NULL));
	SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer);
	
	if (full_screen)
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	makeTemplateOfCircle(radius_of_circle_template);
}

View::~View() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void View::draw(BallSpawner &BSpwn, vector<Line> &lines) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	for (Line line : lines)
		renderLine(line.p1, line.p2, line.color);

	for (Ball ball : BSpwn.balls)
		renderCircle(ball.position, ball.r, ball.color);
	
	SDL_RenderPresent(renderer);
}

void View::renderLine(Vector2D p1, Vector2D p2, RGB color) {
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
	SDL_RenderDrawLine(renderer, p1.x, p1.y, p2.x, p2.y);
}

void View::makeTemplateOfCircle(int r) {
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

void View::drawCircle(int x0, int y0, int radius) {
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

void View::drawFilledCircle(int x0, int y0, int radius) {
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


void View::drawCircleFromTemplate(Vector2D position) {
	std::vector<SDL_Point> temp = templateOfCircle;
	for (int i = 0; i < templateOfCircle.size(); i++) {
		temp[i].x += position.x;
		temp[i].y += position.y;
	}
	SDL_RenderDrawPoints(renderer, temp.data(), temp.size());
}

void View::renderCircle(Vector2D position, int r, RGB color) {
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
	if (r == radius_of_circle_template) {
		drawCircleFromTemplate(position);
	}
	else {
		drawFilledCircle(position.x, position.y, r);
	}
}


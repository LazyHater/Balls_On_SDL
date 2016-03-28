#include "View.h"
#include "Vector2D.h"

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
	for (float y = -r; y <= r; y++)
		for (float x = -r; x <= r; x++)
			if (x*x + y*y < r*r) {
				p.x = x;
				p.y = y;
				templateOfCircle.push_back(p);
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
		for (float y = -r; y <= r; y++)
			for (float x = -r; x <= r; x++)
				if (x*x + y*y < r*r)
					SDL_RenderDrawPoint(renderer, x + position.x, y + position.y);
	}
}


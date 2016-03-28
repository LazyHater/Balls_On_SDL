#pragma once
#include "SDL.h" 
#include "BallSpawner.h"
#include "Line.h"
#include <vector>
#include <ctime>

class View {
public:
	SDL_Renderer *renderer;
	SDL_Window *window;

	std::vector<SDL_Point> templateOfCircle;
	int radius_of_circle_template = 5;

	View(int width, int height, bool full_screen);
	~View();
	void draw(BallSpawner &BSpwn, std::vector<Line> &lines);

	void renderLine(Vector2D p1, Vector2D p2, RGB color);
	void renderCircle(Vector2D position, int r, RGB color);
	void drawCircleFromTemplate(Vector2D position);
	void makeTemplateOfCircle(int r);

};
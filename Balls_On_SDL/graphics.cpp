#include "graphics.h"
#include <vector>

std::vector<SDL_Point> templateOfCircle;
int radius_of_balls = 0;

void renderLine(SDL_Renderer *renderer, Vector2D p1, Vector2D p2, RGB color)
{
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
	SDL_RenderDrawLine(renderer, p1.x, p1.y, p2.x, p2.y);
}

void makeTemplateOfCircle(int r)
{
	templateOfCircle.clear();
	radius_of_balls = r;
	SDL_Point p;
	for (float y = -r; y <= r; y++)
		for (float x = -r; x <= r; x++)
			if (x*x + y*y < r*r)
			{
				p.x = x;
				p.y = y;
				templateOfCircle.push_back(p);
			}
}


void renderCircle(SDL_Renderer *renderer, Vector2D position, int r, RGB color)
{
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
	if (r == radius_of_balls)
	{
		drawCircleFromTemplate(renderer, position);
	}
	else
	{
		for (float y = -r; y <= r; y++)
			for (float x = -r; x <= r; x++)
				if (x*x + y*y < r*r)
					SDL_RenderDrawPoint(renderer, x + position.x, y + position.y);
	}
}

float randTo(float f)
{
	return static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / f));
}

float randFromTo(float from, float to)
{
	return from + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (to - from)));
}

void drawCircleFromTemplate(SDL_Renderer *renderer, Vector2D position)
{
	std::vector<SDL_Point> temp = templateOfCircle;
	for (int i = 0; i < templateOfCircle.size(); i++)
	{
		temp[i].x += position.x;
		temp[i].y += position.y;
	}
	SDL_RenderDrawPoints(renderer, temp.data(), temp.size());
}
#pragma once
#include <vector>
#include <cstdlib>
#include "SDL.h"
#include "Vector2D.h"

struct RGB
{
	int r, g, b;

	RGB() : r(0), g(0), b(0)
	{
		
	}
	RGB(int r, int g, int b) : r(r), g(g), b(b)
	{
	}
	RGB randomize()
	{
		r = rand() % 255;
		g = rand() % 255;
		b = rand() % 255;
		return RGB(r, g, b);
	}

};

struct Point
{
	float x, y;
	Point(): x(0), y(0)
	{
	}
	Point(float x, float y): x(x), y(y)
	{
	}
};
void renderLine(SDL_Renderer *renderer, Vector2D p1, Vector2D p2, RGB color);
void renderCircle(SDL_Renderer *renderer, Vector2D position, int r, RGB color);
void makeTemplateOfCircle(int r);
float randTo(float f);
float randFromTo(float from, float to);
void drawCircleFromTemplate(SDL_Renderer *renderer, Vector2D position);





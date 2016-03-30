#pragma once
#include <cstdlib>
#include"SDL.h"
#include <vector>
#include "Vector2D.h"
#include "Object.h"
float randTo(float f);
float randFromTo(float from, float to);



class Graphic {
public:
	static std::vector<SDL_Point> templateOfCircle;
	static int radius_of_circle_template;

	static void renderCircle(SDL_Renderer *renderer, Vector2D position, int r, RGB color);
	static void drawCircleFromTemplate(SDL_Renderer *renderer, Vector2D position);
	static void makeTemplateOfCircle(int r);
	static void drawCircle(SDL_Renderer *renderer, int x0, int y0, int radius);
	static void drawFilledCircle(SDL_Renderer *renderer, int x0, int y0, int radius);

};




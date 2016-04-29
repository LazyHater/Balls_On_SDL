#pragma once
#include "SDL.h" 
#include "BallSpawner.h"
#include "Line.h"
#include "Vector2D.h"
#include <vector>
#include <ctime>
#include "graphics.h"

class View {
public:
	SDL_Renderer *renderer;
	SDL_Window *window;

	Vector2D position = Vector2D(0, 0);

	View(int width, int height, bool full_screen);
	~View();

	void clear();
	void draw(Object *obj);
	void swapBuffers();
};


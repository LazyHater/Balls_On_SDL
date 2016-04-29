#pragma once
#include <vector>
#include "Line.h"
#include "BallSpawner.h"

class Environment {
	void handleCollisionWithScreen(std::vector<Ball> &balls);
	void handleCollisionWithLines(std::vector<Ball> &balls, std::vector<Line> lines);
	void handleCollisionWithRectangles(std::vector<Ball> &balls, std::vector <Rectangle> &rectangles);
	
public:
	BallSpawner BSpwn;
	std::vector<Line> lines;
	std::vector<Rectangle> rectangles;

	Rectangle box; //area where objects can move and exist
	Vector2D gravity_vector;

	int precision_of_calcs = 1;  //the more the better
	int balls_per_deploy = 100;

	float speed_of_simulation = 1.0f;

	Environment(Rectangle box);

	void update(float delta_t);
};
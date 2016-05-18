#pragma once
#include <vector>
#include "Line.h"
#include "BallSpawner.h"

class Environment {
	void handleCollisionWithScreen(std::vector<Ball> &balls);
	void handleCollisionWithLines(std::vector<Ball> &balls, std::vector<Line> lines);
	void handleCollisionWithRectangles(std::vector<Ball> &balls, std::vector <Rectangle> &rectangles);
	void handleGravityForces(std::vector<Ball> &balls);
public:
	BallSpawner BSpwn;
	std::vector<Line> lines;
	std::vector<Rectangle> rectangles;

	Rectangle box; //area where objects can move and exist
	static Vector2D gravity_vector;  //gravity like on earth 

	static int precision_of_calcs;  //the more the better

	static float speed_of_simulation;
	static bool gravity_forces; // gravity like in solar system

	Environment(Rectangle box);

	void update(float delta_t);
};



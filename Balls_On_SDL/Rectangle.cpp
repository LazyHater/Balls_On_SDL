#include "Rectangle.h"

Rectangle::Rectangle() : width(0), height(0) {}

Rectangle::Rectangle(Vector2D center, float width, float height) :
			width(width), height(height) {
	position = center;
}

Rectangle::Rectangle(Vector2D v1, Vector2D v2) { set(v1, v2); }

void  Rectangle::set(Vector2D v1, Vector2D v2) {
	position = (v1 + v2) / 2.0f;
	width = abs(v1.x - v2.x) / 2.0f;
	height = abs(v1.y - v2.y) / 2.0f;
}


bool Rectangle::isIn(Vector2D v) {
	if ((abs(v.x - position.x) < width) 
		&&(abs(v.y - position.y) < height)) return true;
	return false;
}
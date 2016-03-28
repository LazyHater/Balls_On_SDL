#include "Rectangle.h"

Rectangle::Rectangle() : width(0), height(0)
{

}

Rectangle::Rectangle(Vector2D center, float width, float height) :
	width(width), height(height), center(center)
{

}

Rectangle::Rectangle(Vector2D v1, Vector2D v2) 
{
	set(v1, v2);
}

void  Rectangle::set(Vector2D v1, Vector2D v2)
{
	center = (v1 + v2) / 2.0;
	width = abs(v1.x - v2.x) / 2.0;
	height = abs(v1.y - v2.y) / 2.0;
}


bool Rectangle::isIn(Vector2D v)
{
	if ((abs(v.x - center.x) < width) &&
		(abs(v.y - center.y) < height)) return true;
	return false;
}
#include"Line.h"

Line::Line() : length(0) {}

Line::Line(const Line &l) : p1(l.p1), p2(l.p2) { 
	color = l.color;
	this->update(); 
}

Line::Line(Vector2D p1, Vector2D p2, RGB color) : p1(p1), p2(p2) {
	this->color = color;
	this->update();
}

void Line::update() {
	float dx = (p1.x - p2.x);
	float dy = (p1.y - p2.y);
	
	if (dx == 0)
		collideBox = Rectangle(Vector2D(0, p1.y), Vector2D(5000, p2.y));
	else if (dy == 0)
			collideBox = Rectangle(Vector2D(p1.x,0), Vector2D(p2.x,5000));
		 else
			collideBox = Rectangle(p1, p2); 

	dirVec.set(dx, dy);
	length = dirVec.magnitude();
	norVec = dirVec;
	norVec.rotate(M_PI / 2);
	norVec.normalize();

	if (norVec.angle(Vector2D(0, -1)) > M_PI / 2.0) norVec.negate();
}

float Line::distance(Vector2D v) { // not sure why it works
	float r = (((v.x - p1.x)*(p2.x - p1.x)) + ((v.y - p1.y)*(p2.y - p1.y))) / (length*length);
	float closestX = p1.x + r*(p2.x - p1.x);
	float closestY = p1.y + r*(p2.y - p1.y);
	float distToPointX = closestX - v.x;
	float distToPointY = closestY - v.y;
	float distToPoint = sqrt(distToPointX*distToPointX + distToPointY*distToPointY);
	return distToPoint;
}
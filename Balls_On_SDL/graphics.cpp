#include "graphics.h"

float randTo(float f) {
	return static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / f));
}

float randFromTo(float from, float to)
{
	return from + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (to - from)));
}


#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

View view;

View setPlayercoordinateForView(float x, float y)
{
	float tempX = x; float tempY = y;
	/*if (x < 320) tempX = 320;
	if (y < 240) tempY = 240;*/
	if (y > 624) tempY = 624;

	view.setCenter(tempX, tempY);
	return view;
}


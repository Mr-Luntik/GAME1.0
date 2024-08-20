#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

View view;

View setPlayercoordinateForView(float x, float y)
{
	float tempX = x; float tempY = y;
	
	
	if (x < 180) tempX = 180;
	if (x > 3020) tempX = 3020;
	if (y > 320) tempY = 0;
	if (y < 320) tempY = 280;

	view.setCenter(tempX, tempY);
	return view;
}


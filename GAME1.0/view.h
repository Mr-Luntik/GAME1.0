#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
using namespace sf;
using namespace std;

View view;

View setPlayercoordinateForView(float x, float y)
{
	float tempX = x; float tempY = y;
	
	
	if (x < 180) tempX = 180;
	if (x > 3020) tempX = 3020;
	if (y > 265) tempY = 265;
	if (y < 265) tempY = 265;
	
	view.setCenter(tempX, tempY);
	return view;
}


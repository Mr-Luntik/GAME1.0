#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

View view;

View setPlayercoordinateForView(float x, float y)
{
	float tempX = x; float tempY = y;
	if (x < 320) tempX = 320;
	if (y < 240) tempY = 240;
	if (y > 554) tempY = 554;
	view.setCenter(tempX, tempY);
	return view;
}

View viewmap(float time)
{
	if (Keyboard::isKeyPressed(Keyboard::D))
	{
		view.move(0.1 * time, 0);
	}
	if (Keyboard::isKeyPressed(Keyboard::S))
	{
		view.move(0, 0.1 * time);
	}
	if (Keyboard::isKeyPressed(Keyboard::A))
	{
		view.move(-0.1 * time, 0);
	}
	if (Keyboard::isKeyPressed(Keyboard::W))
	{
		view.move(0, -0.1 * time);
	}
	return view;
}
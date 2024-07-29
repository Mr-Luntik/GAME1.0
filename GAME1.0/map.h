#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;
using namespace std;

const int HEIGHT_MAP = 25;
const int WIDTH_MAP = 40;

sf::String TileMap[HEIGHT_MAP] = {
	"0000000000000000000000000000000000000000",
	"0       s                              0",
	"0   s                                  0",
	"0     s   s                            0",
	"0  s                                   0",
	"0                                      0",
	"0   s     s                            0",
	"0                h                     0",
	"0                                      0",
	"0                                      0",
	"0                          h           0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                           f          0",
	"0             f                        0",
	"0                                      0",
	"0                                      0",
	"0                            f         0",
	"0                                      0",
	"0                                      0",
	"0        f                             0",
	"0                                      0",
	"0000000000000000000000000000000000000000",
};

void randomMapGenerate()
{
	int randomElementX = 0;
	int randomElementY = 0;
	srand(time(0));
	int counterStone = 1;

	while (counterStone > 0)
	{
		randomElementX = 1 + rand() % (WIDTH_MAP - 1);
		randomElementY = 1 + rand() % (HEIGHT_MAP - 1);
		if (TileMap[randomElementY][randomElementX] == ' ')
		{
			TileMap[randomElementY][randomElementX] = 's';
			cout << "coordinate of stone X: " << randomElementX << "\n" << "coordinate of stone Y: " << randomElementY << "\n\n";
			counterStone--;
		}
	}
}

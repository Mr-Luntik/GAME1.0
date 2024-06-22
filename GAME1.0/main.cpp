#include <SFML/Graphics.hpp>
#include "map.h"
#include "view.h"
#include <iostream>
#include <sstream>
using namespace sf;

class Player
{
private: float x, y = 0;
public:
	float w, h, dx, dy, speed = 0;
	int dir, playerScore, health;
	bool life;
	String File;
	Image image;
	Texture texture;
	Sprite sprite;
	Player(String F, int X, int Y, float W, float H)
	{
		dir = 0; speed = 0; playerScore = 0, health = 100;
		life = true;
		File = F; // Имя файла + расширение
		w = W; h = H; //Высота и ширина
		image.loadFromFile("images/" + File);
		image.createMaskFromColor(Color(41, 33, 59)); 
		texture.loadFromImage(image);
		sprite.setTexture(texture); //Заливаем спрайт текстурой
		x = X; y = Y; //Координаты появления спрайта
		sprite.setTextureRect(IntRect(w, h, w, h));
	}
	void update(float time)
	{
		switch (dir)
		{
		case 0: dx = speed; dy = 0; break;
		case 1: dx = -speed; dy = 0; break;
		case 2: dx = 0; dy = speed; break;
		case 3: dx = 0; dy = -speed; break;
		}

		x += dx * time;
		y += dy * time;

		speed = 0;
		sprite.setPosition(x, y);
		interactionWithMap();
		if (health <= 0) // Если жизнь = 0 то герой умирает.
		{
			life = false;
			speed = 0;
		}

	}
		float getPlayercoordinateX()
		{
			return x;
		}
		float getPlayercoordinateY()
		{
			return y;
		}

		void interactionWithMap()//ф-ция взаимодействия с картой
		{

			for (int i = y / 32; i < (y + h) / 32; i++)//проходимся по тайликам, контактирующим с игроком,, то есть по всем квадратикам размера 32*32, которые мы окрашивали в 9 уроке. про условия читайте ниже.
				for (int j = x / 32; j < (x + w) / 32; j++)//икс делим на 32, тем самым получаем левый квадратик, с которым персонаж соприкасается. (он ведь больше размера 32*32, поэтому может одновременно стоять на нескольких квадратах). А j<(x + w) / 32 - условие ограничения координат по иксу. то есть координата самого правого квадрата, который соприкасается с персонажем. таким образом идем в цикле слева направо по иксу, проходя по от левого квадрата (соприкасающегося с героем), до правого квадрата (соприкасающегося с героем)
				{
					if (TileMap[i][j] == '0')//если наш квадратик соответствует символу 0 (стена), то проверяем "направление скорости" персонажа:
					{
						if (dy > 0)//если мы шли вниз,
						{
							y = i * 32 - h;//то стопорим координату игрек персонажа. сначала получаем координату нашего квадратика на карте(стены) и затем вычитаем из высоты спрайта персонажа.
						}
						if (dy < 0)
						{
							y = i * 32 + 32;//аналогично с ходьбой вверх. dy<0, значит мы идем вверх (вспоминаем координаты паинта)
						}
						if (dx > 0)
						{
							x = j * 32 - w;//если идем вправо, то координата Х равна стена (символ 0) минус ширина персонажа
						}
						if (dx < 0)
						{
							x = j * 32 + 32;//аналогично идем влево
						}
					}

					if (TileMap[i][j] == 's') 
					{ //если символ равен 's' (камень)
						playerScore++;//какое то действие... например телепортация героя
						TileMap[i][j] = ' ';//убираем камень, типа взяли бонус. можем и не убирать, кстати.
					}
					if (TileMap[i][j] == 'f')
					{
						health -= 40; //Если взяли едавитый цветок жизнь -40
						TileMap[i][j] = ' ';
					}
					if (TileMap[i][j] == 'h')
					{
						health += 20;
						TileMap[i][j] = ' ';
					}
				}
		}
	
};

int main()
{

	RenderWindow window(sf::VideoMode(640, 480), "Lesson 1. kychka-pc.ru");
	view.reset(FloatRect(0, 0, 640, 480));

	Font font;//шрифт 
	font.loadFromFile("CyrilicOld.ttf");//передаем нашему шрифту файл шрифта
	Text text("", font, 20);//создаем объект текст. закидываем в объект текст строку, шрифт, размер шрифта(в пикселях);//сам объект текст (не строка)
	text.setFillColor (Color::Red);//покрасили текст в красный. если убрать эту строку, то по умолчанию он белый
	text.setStyle(sf::Text::Bold | sf::Text::Underlined);

	Image heroimog;
	heroimog.loadFromFile("./images/hero.png");

	Image map_image;
	map_image.loadFromFile("./images/map.png");
	Texture map;
	map.loadFromImage(map_image);
	Sprite s_map;
	s_map.setTexture(map);
	Player p("hero.png", 250, 250, 96.0, 96.0);

	float CurrentFrame = 0;
	Clock clock;
	Clock gameTimeClock;
	int gameTime = 0;


	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds();
		if (p.life) gameTime = gameTimeClock.getElapsedTime().asSeconds();//игровое время в секундах идёт вперед, пока жив игрок, перезагружать как time его не надо. оно не обновляет логику игры
		clock.restart();
		time = time / 800;

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		
		if (p.life)
		{
			if ((Keyboard::isKeyPressed(Keyboard::Left)))
			{
				p.dir = 1; p.speed = 0.1;
				CurrentFrame += 0.005 * time;
				if (CurrentFrame > 3) CurrentFrame -= 3;
				p.sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 96, 96, 96));

			}

			if ((Keyboard::isKeyPressed(Keyboard::Right)))
			{
				p.dir = 0; p.speed = 0.1;
				CurrentFrame += 0.005 * time;
				if (CurrentFrame > 3) CurrentFrame -= 3;
				p.sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 192, 96, 96));

			}

			if ((Keyboard::isKeyPressed(Keyboard::Up)))
			{
				p.dir = 3; p.speed = 0.1;
				CurrentFrame += 0.005 * time;
				if (CurrentFrame > 3) CurrentFrame -= 3;
				p.sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 307, 96, 96));

			}

			if ((Keyboard::isKeyPressed(Keyboard::Down)))
			{
				p.dir = 2; p.speed = 0.1;
				CurrentFrame += 0.005 * time;
				if (CurrentFrame > 3) CurrentFrame -= 3;
				p.sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 0, 96, 96));

			}
		}
		
		getPlayercoordinateForView(p.getPlayercoordinateX(), p.getPlayercoordinateY());

		p.update(time);
		viewmap(time);
		window.setView(view);
		window.clear();
		//Рисуем карту
		for (int i = 0; i < HEIGHT_MAP; i++)
			for (int j = 0; j < WIDTH_MAP; j++)
			{
				if (TileMap[i][j] == ' ')  s_map.setTextureRect(IntRect(0, 0, 32, 32)); //если встретили символ пробел, то рисуем 1й квадратик
				if (TileMap[i][j] == 's')  s_map.setTextureRect(IntRect(32, 0, 32, 32));//если встретили символ s, то рисуем 2й квадратик
				if ((TileMap[i][j] == '0')) s_map.setTextureRect(IntRect(64, 0, 32, 32));//если встретили символ 0, то рисуем 3й квадратик
				if ((TileMap[i][j] == 'f')) s_map.setTextureRect(IntRect(96, 0, 32, 32)); //Добавляем уветок
				if ((TileMap[i][j] == 'h')) s_map.setTextureRect(IntRect(128, 0, 32, 32)); //Добавляем Сердечко


				s_map.setPosition(j * 32, i * 32);//по сути раскидывает квадратики, превращая в карту. то есть задает каждому из них позицию. если убрать, то вся карта нарисуется в одном квадрате 32*32 и мы увидим один квадрат

				window.draw(s_map);//рисуем квадратики на экран
			}
		std::ostringstream playerHealtString, gameTimeString;
		
		playerHealtString << p.health; gameTimeString << gameTime;		//формируем строку
		text.setString("Здоровье: " + playerHealtString.str() + "\nВремя игры: " + gameTimeString.str());
		text.setPosition(view.getCenter().x - 165, view.getCenter().y - 200);
		window.draw(text);

		window.draw(p.sprite);
		
		window.display();
	}

	return 0;
}
#include <SFML/Graphics.hpp>
//#include "map.h"
#include "view.h"
#include <iostream>
#include <sstream>
#include "mission.h"
#include <vector>
//#include <list>
#include "Level.h"

using namespace sf;
using namespace std;

class Entity {
public:
	vector<Object>obj;
	float dx, dy, x, y, speed, moveTimer;//добавили переменную таймер для будущих целей
	int w, h, health, dir;
	bool life, isMove, onGround;
	Texture texture;
	Sprite sprite;
	String name;//враги могут быть разные, мы не будем делать другой класс для различающегося врага.всего лишь различим врагов по имени и дадим каждому свое действие в update в зависимости от имени
	Entity(Image& image, String Name, Level &Lvl, float X, float Y, int W, int H) {
		x = X; y = Y; w = W; h = H; name = Name; moveTimer = 0;
		speed = 0; health = 100; dx = 0; dy = 0, dir = 0;
		life = true; onGround = false; isMove = false;
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setOrigin(w / 2, h / 2);
	}
	FloatRect getRect()
	{
		return FloatRect(x, y, w, h);
	}
};
////////////////////////////////////////////////////КЛАСС ИГРОКА////////////////////////
class Player :public Entity {
public:
	enum { left, right, up, down, jump, stay } state;//добавляем тип перечисления - состояние объекта
	int playerScore;//эта переменная может быть только у игрока

	Player(Image& image, String Name, Level &lvl, float X, float Y, int W, int H) :Entity(image, Name, lvl, X, Y, W, H) {
		playerScore = 0; state = stay; obj = lvl.GetAllObjects();
		if (name == "Player1") {
			sprite.setTextureRect(IntRect(4, 19, w, h));
		}
	}

	void control()
	{
		if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			state = left; speed = 0.1;
		}
		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			state = right, speed = 0.1;
		}
		if ((Keyboard::isKeyPressed(Keyboard::Up)) && (onGround))
		{
			state = jump; dy = -0.6; onGround = false;
		}
		if (Keyboard::isKeyPressed(Keyboard::Down))
		{
			state = down;
		}
	}

	void checkCollisionWithMap(float Dx, float Dy)//ф-ция взаимодействия с картой
	{
		//for (int i = y / 32; i < (y + h) / 32; i++)//проходимся по тайликам, контактирующим с игроком,, то есть по всем квадратикам размера 32*32, которые мы окрашивали в 9 уроке. про условия читайте ниже.
		//	for (int j = x / 32; j < (x + w) / 32; j++)//икс делим на 32, тем самым получаем левый квадратик, с которым персонаж соприкасается. (он ведь больше размера 32*32, поэтому может одновременно стоять на нескольких квадратах). А j<(x + w) / 32 - условие ограничения координат по иксу. то есть координата самого правого квадрата, который соприкасается с персонажем. таким образом идем в цикле слева направо по иксу, проходя по от левого квадрата (соприкасающегося с героем), до правого квадрата (соприкасающегося с героем)
		//	{
		//		if (TileMap[i][j] == '0')//если наш квадратик соответствует символу 0 (стена), то проверяем "направление скорости" персонажа:
		//		{
		//			if (Dy > 0) { y = i * 32 - h; Dy = 0; onGround = true; }//если мы шли вниз
		//			//то стопорим координату игрек персонажа. сначала получаем координату нашего квадратика на карте(стены) и затем вычитаем из высоты спрайта персонажа
		//			if (Dy < 0) { y = i * 32 + 32; Dy = 0; }//аналогично с ходьбой вверх. dy<0, значит мы идем вверх (вспоминаем координаты
		//			if (Dx > 0) { x = j * 32 - w; }//если идем вправо, то координата Х равна стена (символ 0) минус ширина
		//			if (Dx < 0) { x = j * 32 + 32; } //аналогично идем влево
		//		}
		//		/*else { onGround = false; }*/
		//	}
		for (int i = 0; i < obj.size(); i++)//проходимся по объектам
			if (getRect().intersects(obj[i].rect))//проверяем пересечение игрока с объектом
			{
				if (obj[i].name == "solid")//если встретили препятствие
				{
					if (Dy > 0) { y = obj[i].rect.top - h;  dy = 0; onGround = true; }
					if (Dy < 0) { y = obj[i].rect.top + obj[i].rect.height;   dy = 0; }
					if (Dx > 0) { x = obj[i].rect.left - w; }
					if (Dx < 0) { x = obj[i].rect.left + obj[i].rect.width; }
				}
			}
	}

	void update(float time)
	{
		control();
		switch (state)
		{
		case right: dx = speed; break;
		case left: dx = -speed; break;
		case up: break;
		case down: dx = 0; break;
		case stay: break;
		}
		x += dx * time;
		checkCollisionWithMap(dx, 0);
		y += dy * time;
		checkCollisionWithMap(0, dy);

		sprite.setPosition(x + w / 2, y + h / 2);
		if (health <= 0) { life = false; }
		if (!isMove) { speed = 0; }
		if (life) { setPlayercoordinateForView(x, y); }
		dy = dy + 0.0015 * time;
		/*if (!onGround) { dy = dy + 0.0015 * time; }*/
	}
	/*float getPlayercoordinateX()
	{
		return x;
	}
	float getPlayercoordinateY()
	{
		return y;
	}*/
};

class Enemy :public Entity
{
public:
	Enemy(Image& image, String Name, Level &lvl, float X, float Y, int W, int H) : Entity(image, Name, lvl, X, Y, W, H )
	{
		obj = lvl.GetAllObjects();
		if (name == "EasyEnemy")
		{
			sprite.setTextureRect(IntRect(0, 0, w, h));
			dx = 0.1;
		}
	}

	void checkCollisionWithMap(float Dx, float Dy)//ф ция проверки столкновений с картой
	{
		for (int i = 0; i < obj.size(); i++)//проходимся по объектам
			if (getRect().intersects(obj[i].rect))//проверяем пересечение игрока с объектом
			{
				//if (obj[i].name == "solid")//если встретили препятствие
				//{
					if (Dy > 0) { y = obj[i].rect.top - h;  dy = 0; onGround = true; }
					if (Dy < 0) { y = obj[i].rect.top + obj[i].rect.height;   dy = 0; }
					if (Dx > 0) { x = obj[i].rect.left - w;  dx = -0.1; sprite.scale(-1, 1); }
					if (Dx < 0) { x = obj[i].rect.left + obj[i].rect.width; dx = 0.1; sprite.scale(-1, 1); }
				/*}*/
			}
	}

	void update(float time)
	{
		if (name == "EasyEnemy") {//для персонажа с таким именем логика будет такой

			//moveTimer += time;if (moveTimer>3000){ dx *= -1; moveTimer = 0; }//меняет направление примерно каждые 3 сек
			checkCollisionWithMap(dx, 0);//обрабатываем столкновение по Х
			x += dx * time;
			sprite.setPosition(x + w / 2, y + h / 2); //задаем позицию спрайта в место его центра
			if (health <= 0) { life = false; }
		}
	}
};

int main()
{
	RenderWindow window(sf::VideoMode(640, 480), "Lesson 1. kychka-pc.ru"/*, Style::Fullscreen*/);
	view.reset(FloatRect(0, 0, 640, 480));

	Level lvl;
	lvl.LoadFromFile("FilesTheTiles/Map.tmx");

	/*Image map_image;
	map_image.loadFromFile("images/map.png");
	Texture map;
	map.loadFromImage(map_image);
	Sprite s_map;
	s_map.setTexture(map);*/

	Image easyEnemyImage;
	easyEnemyImage.loadFromFile("images/shamaich.png");
	easyEnemyImage.createMaskFromColor(Color(255, 0, 0));//сделали маску по цвету.но лучше изначально иметь прозрачную картинку

	Image heroImage;
	heroImage.loadFromFile("images/MilesTailsPrower.gif");

	Object player = lvl.GetObject("player");
	Object easyEnemyObject = lvl.GetObject("easyEnemy");

	Player p(heroImage, "Player1", lvl, player.rect.left, player.rect.top, 40, 30);//передаем координаты прямоугольника player из карты в координаты нашего игрока
	Enemy easyEnemy(easyEnemyImage, "EasyEnemy", lvl, easyEnemyObject.rect.left, easyEnemyObject.rect.top, 200, 97);//передаем координаты прямоугольника easyEnemy из карты в координаты нашего врага

	Clock clock;
	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 800;

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		if ((Keyboard::isKeyPressed(Keyboard::Escape)))
		{
			window.close();
		}

		p.update(time);
		easyEnemy.update(time);
		window.setView(view);
		window.clear(Color(77, 83, 140));
		lvl.Draw(window);

		//Рисуем карту
		/*for (int i = 0; i < HEIGHT_MAP; i++)
			for (int j = 0; j < WIDTH_MAP; j++)
			{
				if (TileMap[i][j] == ' ')  s_map.setTextureRect(IntRect(0, 0, 32, 32));
				if (TileMap[i][j] == 's')  s_map.setTextureRect(IntRect(32, 0, 32, 32));
				if ((TileMap[i][j] == '0')) s_map.setTextureRect(IntRect(64, 0, 32, 32));
				if ((TileMap[i][j] == 'f')) s_map.setTextureRect(IntRect(96, 0, 32, 32));
				if ((TileMap[i][j] == 'h')) s_map.setTextureRect(IntRect(128, 0, 32, 32));
				s_map.setPosition(j * 32, i * 32);

				window.draw(s_map);
			}*/
		window.draw(easyEnemy.sprite);
		window.draw(p.sprite);
		window.display();
	}
	return 0;
}
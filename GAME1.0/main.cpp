#include <SFML/Graphics.hpp>
#include "view.h"
#include <iostream>
#include <sstream>
#include "mission.h"
#include <windows.h>
#include "iostream"
#include "level.h"
#include <vector>
#include <list>
#include <chrono>
#include <locale.h>
#include <thread>
#include "TinyXML/tinyxml.h"
using namespace sf;

////////////////////////////////////Общий класс-родитель//////////////////////////
class Entity {
public:
	std::vector<Object> obj;//вектор объектов карты
	float dx, dy, x, y, speed, moveTimer, deltaTime, elapsedTime, respawnTime, currentFrame, currentFrameEnemy, isInvulnerable;
	const int invulnerabilityDuration = 2;
	const float CAMERA_MARGIN_X = 100.0f, CAMERA_MARGIN_Y = 50.0f;
	int w, h, health;
	bool life, isMove, onGround, enemyAlive, isKeyPressed;
	Texture texture;
	Sprite sprite;
	String name;
	Entity(Image& image, String Name, float X, float Y, int W, int H) {
		x = X; y = Y; w = W; h = H; name = Name; moveTimer = 0, elapsedTime = 0, respawnTime = 3000, currentFrame = 0, currentFrameEnemy = 0; 
		speed = 0; health = 100; dx = 0; dy = 0;
		life = true; onGround = false; isMove = false; enemyAlive = true; isInvulnerable = false; isKeyPressed = false;
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setOrigin(w / 2, h / 2);
		sprite.scale(0.7f, 0.7f);
	}
	FloatRect getRect() {//ф-ция получения прямоугольника. его коорд,размеры (шир,высот).
		return FloatRect(x, y, w, h);//эта ф-ция нужна для проверки столкновений
	}
	virtual void update(float time) = 0;
};
////////////////////////////////////////////////////КЛАСС ИГРОКА////////////////////////
class Player :public Entity {
public:
	enum { left, right, up, down, jump, stay } state;
	int playerScore;
	Player(Image& image, String Name, Level& lev, float X, float Y, int W, int H) :Entity(image, Name, X, Y, W, H) {
		playerScore = 0; state = stay; obj = lev.GetAllObjects();//инициализируем.получаем все объекты для взаимодействия персонажа с картой
		if (name == "Player1") {
			sprite.setTextureRect(IntRect(52, 2, w, h));
		}
		sprite.scale(0.5f, 0.6f);
	}
	/*FloatRect getRect()
	{
		return FloatRect(x, y, w, h);
	}*/

	void control() {
		/*if (isKeyPressed)
		{*/
			if (Keyboard::isKeyPressed(Keyboard::Left)) {
				state = left; speed = 0.05;
				sprite.setTextureRect(IntRect(55, 77, w, h));

			}
			if (Keyboard::isKeyPressed(Keyboard::Right)) {
				state = right; speed = 0.05;
				sprite.setTextureRect(IntRect(50, 149, w, h));
			}
			if ((Keyboard::isKeyPressed(Keyboard::Up)) && (onGround)) {
				state = jump; dy = -0.5; onGround = false;
			}
			if (Keyboard::isKeyPressed(Keyboard::Down)) {
				state = down;
			}
		/*}*/
		
	}
	void checkCollisionWithMap(float Dx, float Dy)
	{
		for (int i = 0; i < obj.size(); i++)
		{//проходимся по объектам
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
			else { if (dy > 0) { onGround = false; } }
		}
	}
	void update(float time)
	{
		currentFrame += 0.005 * time;
		if (currentFrame > 3)
		{
			currentFrame -= 3;
		}
		control();
		switch (state)
		{
		case right:dx = speed; if (speed > 0) { sprite.setTextureRect(IntRect(50.5 * int(currentFrame), 149, 41, 64)); } break;
		case left:dx = -speed; if (speed > 0) { sprite.setTextureRect(IntRect(50.5 * int(currentFrame), 77, 41, 64)); } break;
		case up: break;
		case down: dx = 0; break;
		case stay: break;
		}
		x += dx * time;
		checkCollisionWithMap(dx, 0);
		y += dy * time;
		checkCollisionWithMap(0, dy);
		sprite.setPosition(x + w / 1.9, y + h / 1.25);
		if (health <= 0) { life = false; }
		if (!isMove) { speed = 0; }
		
		if (life) { /*setPlayercoordinateForView(x, y); */}
		dy = dy + 0.0015 * time;
	}

	//Функция нанесения урона по герою
	void takeDamage(int damage)
	{
		if (isInvulnerable)
		{
			return;
		}
		health -= damage;

		if (health < 0)
		{
			health = 0;
		}
		makeInvulnerable();
	}
	void makeInvulnerable()
	{
		isInvulnerable = true;
		std::thread([this]() {
			std::this_thread::sleep_for(std::chrono::seconds(invulnerabilityDuration));
			isInvulnerable = false;
			}).detach();

	}
};
class Enemy :public Entity {
public:
	Enemy(Image& image, String Name, Level& lvl, float X, float Y, int W, int H) :Entity(image, Name, X, Y, W, H) {
		obj = lvl.GetObjects("solid");//инициализируем.получаем нужные объекты для взаимодействия врага с картой
		if (name == "EasyEnemy") {
			sprite.setTextureRect(IntRect(4, 142, w, h));
			dx = 0.02;
			sprite.scale(1.0f, 1.0f);

		}
	}
	void checkCollisionWithMap(float Dx, float Dy)
	{
		for (int i = 0; i < obj.size(); i++)//проходимся по объектам
			if (getRect().intersects(obj[i].rect))//проверяем пересечение игрока с объектом
			{
				//if (obj[i].name == "solid"){//если встретили препятствие (объект с именем solid)
				if (Dy > 0) { y = obj[i].rect.top - h;  dy = 0; onGround = true; }
				if (Dy < 0) { y = obj[i].rect.top + obj[i].rect.height;   dy = 0; }
				if (Dx > 0) { x = obj[i].rect.left - w;  dx = -0.02; sprite.scale(-1, 1); }
				if (Dx < 0) { x = obj[i].rect.left + obj[i].rect.width; dx = 0.02; sprite.scale(-1, 1); }
				//}
			}
	}
	void update(float time)
	{
		if (name == "EasyEnemy") {
			//moveTimer += time;if (moveTimer>3000){ dx *= -1; moveTimer = 0; }//меняет направление примерно каждые 3 сек
			checkCollisionWithMap(dx, 0);
			x += dx * time;
			sprite.setPosition(x + w / 2, y + h / 1.5);
			if (health <= 0) { life = false; }
		}
	}
};
int main()
{
	setlocale(LC_ALL, "Rus");
	RenderWindow window(VideoMode(1920, 1080), "Lesson 22. kychka-pc.ru", Style::Fullscreen);
	view.reset(FloatRect(0, 0, 320, 240));
	Level lvl;//создали экземпляр класса уровень
	lvl.LoadFromFile("MapForGame1.tmx");//загрузили в него карту, внутри класса с помощью методов он ее обработает.

	Font MyFont;
	MyFont.loadFromFile("SharkyspotDemo.ttf");
	Text text("", MyFont, 10);
	text.setFillColor(Color::Red);
	text.setStyle(sf::Text::Underlined);

	Image heroImage;
	heroImage.loadFromFile("images/mage.png");
	Image easyEnemyImage;
	easyEnemyImage.loadFromFile("images/Skeleton enemy.png");

	Object player = lvl.GetObject("player");//объект игрока на нашей карте.задаем координаты игроку в начале при помощи него

	Player p(heroImage, "Player1", lvl, player.rect.left, player.rect.top, 41, 68);//передаем координаты прямоугольника player из карты в координаты нашего игрока


	std::list<Entity*> entities;
	std::list <Entity*> ::iterator it;
	std::vector<Object> e = lvl.GetObjects("EasyEnemy");
	for (int i = 0; i < e.size(); i++)//проходимся по элементам этого вектора(а именно по врагам)
		entities.push_back(new Enemy(easyEnemyImage, "EasyEnemy", lvl, e[i].rect.left, e[i].rect.top, 39, 36));//и закидываем в список всех наших врагов с карты

	
	


	Clock clock;

	float respawnTimer = 3000, elapsedTime = 0;
	while (window.isOpen())
	{

		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			window.close();
		}

		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 800;

		
		Vector2f playerPosition(p.x, p.y);
		Vector2f viewCenter = view.getCenter();
		if (p.isMove) {
			// Check if the player is near the left edge
			if (playerPosition.x < viewCenter.x - p.CAMERA_MARGIN_X)  {
				view.move(-(viewCenter.x - playerPosition.x + p.CAMERA_MARGIN_X) * time / 1000.0f, 0);
			}
			// Check if the player is near the right edge
			else if (playerPosition.x > viewCenter.x + p.CAMERA_MARGIN_X ) {
				view.move((playerPosition.x - viewCenter.x + p.CAMERA_MARGIN_X) * time / 1000.0f, 0);
			}

			// Check if the player is near the top edge
			if (playerPosition.y < viewCenter.y - p.CAMERA_MARGIN_Y) {
				view.move(0, -(viewCenter.y - playerPosition.y + p.CAMERA_MARGIN_Y) * time / 1000.0f);
			}
			// Check if the player is near the bottom edge
			else if (playerPosition.y > viewCenter.y + p.CAMERA_MARGIN_Y) {
				view.move(0, (playerPosition.y - viewCenter.y + p.CAMERA_MARGIN_Y) * time / 1000.0f);
			}
		}
		sf::Vector2i localPosition(p.x, p.y);
		if (localPosition.x < 3) { view.move(-0.2 * time, 0); }
		if (localPosition.x > window.getSize().x - 3) { view.move(130 * time, 0); }
		if (localPosition.y > window.getSize().y - 3) { view.move(0, 170 * time); }
		if (localPosition.y < 3) { view.move(0, 0.2 * time); }
		


		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		if (event.type == sf::Event::KeyPressed)
		{
			p.isKeyPressed = true;
			p.isMove = true;
		
		}
		if (event.type == sf::Event::KeyReleased)
		{
			p.isKeyPressed = false;
			p.isMove = false;
		}
		p.update(time);
		for (it = entities.begin(); it != entities.end();)
		{

			Entity* b = *it;
			b->update(time);
			if (b->life == true)
			{

				p.currentFrameEnemy += 0.005 * time;
				if (p.currentFrameEnemy > 12) { p.currentFrameEnemy -= 12; }
				b->sprite.setTextureRect((IntRect(64 * int(p.currentFrameEnemy), 141, 42, 37)));
			}
			if (b->life == false)
			{

				it = entities.erase(it);
				delete b;

				entities.push_back(new Enemy(easyEnemyImage, "EasyEnemy", lvl, 900, 750, 39, 36));
				b->life = true;
			}
			else it++;
		}


		for (it = entities.begin(); it != entities.end(); it++)
		{
			if ((*it)->getRect().intersects(p.getRect()))
			{
				if ((*it)->name == "EasyEnemy")
				{
					if ((*it)->dx > 0)
					{
						(*it)->x = p.x - (*it)->w; //отталкиваем его от игрока влево (впритык)
						(*it)->dx = 0;//останавливаем
						if (p.dx < 0) { p.x = (*it)->x + (*it)->w; }
					}
					if ((*it)->dx < 0)//если враг идет влево
					{
						(*it)->x = p.x + p.w; //аналогично - отталкиваем вправо
						(*it)->dx = 0;//останавливаем
					}
					if ((p.dy > 0) && (p.onGround == false)) { (*it)->dx = 0; p.dy = -0.2; (*it)->health = 0; }
					else
					{
						p.takeDamage(5);

					}
					/////выталкивание игрока
					if (p.dx < 0) { p.x = (*it)->x + (*it)->w; }//если столкнулись с врагом и игрок идет влево то выталкиваем игрока
					if (p.dx > 0) { p.x = (*it)->x - p.w; }//если столкнулись с врагом и игрок идет вправо то выталкиваем игрока

				}

			}
		}
		for (it = entities.begin(); it != entities.end(); it++)
			window.setView(view);
		window.clear(Color(77, 83, 140));
		lvl.Draw(window);//рисуем новую карту

		for (it = entities.begin(); it != entities.end(); it++) {
			window.draw((*it)->sprite);
		}
		std::ostringstream playerLife;
		playerLife << p.health;
		text.setString("Life: " + playerLife.str());
		text.setPosition(view.getCenter().x - 150, view.getCenter().y - 120);
		window.draw(text);

		window.draw(p.sprite);
		window.display();
	}
	return 0;
}
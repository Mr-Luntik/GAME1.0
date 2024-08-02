#include <SFML/Graphics.hpp>
#include "map.h"
#include "view.h"
#include <iostream>
#include <sstream>
#include "mission.h"
using namespace sf;
using namespace std;

class Entity {
public:
	float dx, dy, x, y, speed, moveTimer;//�������� ���������� ������ ��� ������� �����
	int w, h, health, dir;
	bool life, isMove, onGround;
	Texture texture;
	Sprite sprite;
	String name;//����� ����� ���� ������, �� �� ����� ������ ������ ����� ��� �������������� �����.����� ���� �������� ������ �� ����� � ����� ������� ���� �������� � update � ����������� �� �����
	Entity(Image& image, float X, float Y, int W, int H, String Name) {
		x = X; y = Y; w = W; h = H; name = Name; moveTimer = 0;
		speed = 0; health = 100; dx = 0; dy = 0, dir = 0;
		life = true; onGround = false; isMove = false;
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setOrigin(w / 2, h / 2);
	}
};
////////////////////////////////////////////////////����� ������////////////////////////
class Player :public Entity {
public:
	enum { left, right, up, down, jump, stay } state;//��������� ��� ������������ - ��������� �������
	int playerScore;//��� ���������� ����� ���� ������ � ������

	Player(Image& image, float X, float Y, int W, int H, String Name) :Entity(image, X, Y, W, H, Name) {
		playerScore = 0; state = stay;
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

	void checkCollisionWithMap(float Dx, float Dy)//�-��� �������������� � ������
	{
		for (int i = y / 32; i < (y + h) / 32; i++)//���������� �� ��������, �������������� � �������,, �� ���� �� ���� ����������� ������� 32*32, ������� �� ���������� � 9 �����. ��� ������� ������� ����.
			for (int j = x / 32; j < (x + w) / 32; j++)//��� ����� �� 32, ��� ����� �������� ����� ���������, � ������� �������� �������������. (�� ���� ������ ������� 32*32, ������� ����� ������������ ������ �� ���������� ���������). � j<(x + w) / 32 - ������� ����������� ��������� �� ����. �� ���� ���������� ������ ������� ��������, ������� ������������� � ����������. ����� ������� ���� � ����� ����� ������� �� ����, ������� �� �� ������ �������� (���������������� � ������), �� ������� �������� (���������������� � ������)
			{
				if (TileMap[i][j] == '0')//���� ��� ��������� ������������� ������� 0 (�����), �� ��������� "����������� ��������" ���������:
				{
					if (Dy > 0) { y = i * 32 - h; Dy = 0; onGround = true; }//���� �� ��� ����
					//�� �������� ���������� ����� ���������. ������� �������� ���������� ������ ���������� �� �����(�����) � ����� �������� �� ������ ������� ���������
					if (Dy < 0) { y = i * 32 + 32; Dy = 0; }//���������� � ������� �����. dy<0, ������ �� ���� ����� (���������� ����������
					if (Dx > 0) { x = j * 32 - w; }//���� ���� ������, �� ���������� � ����� ����� (������ 0) ����� ������
					if (Dx < 0) { x = j * 32 + 32; } //���������� ���� �����
				}
				/*else { onGround = false; }*/
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
	Enemy(Image& image, float X, float Y, int W, int H, String Name) : Entity(image, X, Y, W, H, Name)
	{
		if (name == "EasyEnemy")
		{
			sprite.setTextureRect(IntRect(0, 0, w, h));
			dx = 0.1;
		}
	}

	void checkCollisionWithMap(float Dx, float Dy)//� ��� �������� ������������ � ������
	{
		for (int i = y / 32; i < (y + h) / 32; i++)//���������� �� ��������� �����
			for (int j = x / 32; j < (x + w) / 32; j++)
			{
				if (TileMap[i][j] == '0')//���� ������� ��� ������ �����, ��
				{
					if (Dy > 0) { y = i * 32 - h; }//�� Y ����=>���� � ���(����� �� �����) ��� ������. � ���� ������ ���� ���������� ��������� � ��������� ��� �� �����, ��� ���� ������� ��� �� �� ����� ��� ����� ����� ����� �������
					if (Dy < 0) { y = i * 32 + 32; }//������������ � �������� ������ �����(����� � �� �����������)
					if (Dx > 0) { x = j * 32 - w; dx = -0.1; sprite.scale(-1, 1); }//� ������ ����� �����
					if (Dx < 0) { x = j * 32 + 32; dx = 0.1; sprite.scale(-1, 1); }// � ����� ����� �����
				}
			}
	}

	void update(float time)
	{
		if (name == "EasyEnemy") {//��� ��������� � ����� ������ ������ ����� �����

			//moveTimer += time;if (moveTimer>3000){ dx *= -1; moveTimer = 0; }//������ ����������� �������� ������ 3 ���
			checkCollisionWithMap(dx, 0);//������������ ������������ �� �
			x += dx * time;
			sprite.setPosition(x + w / 2, y + h / 2); //������ ������� ������� � ����� ��� ������
			if (health <= 0) { life = false; }
		}
	}
};

int main()
{
	RenderWindow window(sf::VideoMode(640, 480), "Lesson 1. kychka-pc.ru"/*, Style::Fullscreen*/);
	view.reset(FloatRect(0, 0, 640, 480));

	Image map_image;
	map_image.loadFromFile("images/map.png");
	Texture map;
	map.loadFromImage(map_image);
	Sprite s_map;
	s_map.setTexture(map);

	Image easyEnemyImage;
	easyEnemyImage.loadFromFile("images/shamaich.png");
	easyEnemyImage.createMaskFromColor(Color(255, 0, 0));//������� ����� �� �����.�� ����� ���������� ����� ���������� ��������

	Image heroImage;
	heroImage.loadFromFile("images/MilesTailsPrower.gif");

	Player p(heroImage, 750, 500, 40, 30, "Player1");//������ ������ ������
	Enemy easyEnemy(easyEnemyImage, 850, 671, 200, 97, "EasyEnemy");

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
		window.clear();

		//������ �����
		for (int i = 0; i < HEIGHT_MAP; i++)
			for (int j = 0; j < WIDTH_MAP; j++)
			{
				if (TileMap[i][j] == ' ')  s_map.setTextureRect(IntRect(0, 0, 32, 32));
				if (TileMap[i][j] == 's')  s_map.setTextureRect(IntRect(32, 0, 32, 32));
				if ((TileMap[i][j] == '0')) s_map.setTextureRect(IntRect(64, 0, 32, 32));
				if ((TileMap[i][j] == 'f')) s_map.setTextureRect(IntRect(96, 0, 32, 32));
				if ((TileMap[i][j] == 'h')) s_map.setTextureRect(IntRect(128, 0, 32, 32));
				s_map.setPosition(j * 32, i * 32);

				window.draw(s_map);
			}
		window.draw(easyEnemy.sprite);
		window.draw(p.sprite);
		window.display();
	}
	return 0;
}
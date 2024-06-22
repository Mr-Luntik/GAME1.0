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
		File = F; // ��� ����� + ����������
		w = W; h = H; //������ � ������
		image.loadFromFile("images/" + File);
		image.createMaskFromColor(Color(41, 33, 59)); 
		texture.loadFromImage(image);
		sprite.setTexture(texture); //�������� ������ ���������
		x = X; y = Y; //���������� ��������� �������
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
		if (health <= 0) // ���� ����� = 0 �� ����� �������.
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

		void interactionWithMap()//�-��� �������������� � ������
		{

			for (int i = y / 32; i < (y + h) / 32; i++)//���������� �� ��������, �������������� � �������,, �� ���� �� ���� ����������� ������� 32*32, ������� �� ���������� � 9 �����. ��� ������� ������� ����.
				for (int j = x / 32; j < (x + w) / 32; j++)//��� ����� �� 32, ��� ����� �������� ����� ���������, � ������� �������� �������������. (�� ���� ������ ������� 32*32, ������� ����� ������������ ������ �� ���������� ���������). � j<(x + w) / 32 - ������� ����������� ��������� �� ����. �� ���� ���������� ������ ������� ��������, ������� ������������� � ����������. ����� ������� ���� � ����� ����� ������� �� ����, ������� �� �� ������ �������� (���������������� � ������), �� ������� �������� (���������������� � ������)
				{
					if (TileMap[i][j] == '0')//���� ��� ��������� ������������� ������� 0 (�����), �� ��������� "����������� ��������" ���������:
					{
						if (dy > 0)//���� �� ��� ����,
						{
							y = i * 32 - h;//�� �������� ���������� ����� ���������. ������� �������� ���������� ������ ���������� �� �����(�����) � ����� �������� �� ������ ������� ���������.
						}
						if (dy < 0)
						{
							y = i * 32 + 32;//���������� � ������� �����. dy<0, ������ �� ���� ����� (���������� ���������� ������)
						}
						if (dx > 0)
						{
							x = j * 32 - w;//���� ���� ������, �� ���������� � ����� ����� (������ 0) ����� ������ ���������
						}
						if (dx < 0)
						{
							x = j * 32 + 32;//���������� ���� �����
						}
					}

					if (TileMap[i][j] == 's') 
					{ //���� ������ ����� 's' (������)
						playerScore++;//����� �� ��������... �������� ������������ �����
						TileMap[i][j] = ' ';//������� ������, ���� ����� �����. ����� � �� �������, ������.
					}
					if (TileMap[i][j] == 'f')
					{
						health -= 40; //���� ����� �������� ������ ����� -40
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

	Font font;//����� 
	font.loadFromFile("CyrilicOld.ttf");//�������� ������ ������ ���� ������
	Text text("", font, 20);//������� ������ �����. ���������� � ������ ����� ������, �����, ������ ������(� ��������);//��� ������ ����� (�� ������)
	text.setFillColor (Color::Red);//��������� ����� � �������. ���� ������ ��� ������, �� �� ��������� �� �����
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
		if (p.life) gameTime = gameTimeClock.getElapsedTime().asSeconds();//������� ����� � �������� ��� ������, ���� ��� �����, ������������� ��� time ��� �� ����. ��� �� ��������� ������ ����
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
		//������ �����
		for (int i = 0; i < HEIGHT_MAP; i++)
			for (int j = 0; j < WIDTH_MAP; j++)
			{
				if (TileMap[i][j] == ' ')  s_map.setTextureRect(IntRect(0, 0, 32, 32)); //���� ��������� ������ ������, �� ������ 1� ���������
				if (TileMap[i][j] == 's')  s_map.setTextureRect(IntRect(32, 0, 32, 32));//���� ��������� ������ s, �� ������ 2� ���������
				if ((TileMap[i][j] == '0')) s_map.setTextureRect(IntRect(64, 0, 32, 32));//���� ��������� ������ 0, �� ������ 3� ���������
				if ((TileMap[i][j] == 'f')) s_map.setTextureRect(IntRect(96, 0, 32, 32)); //��������� ������
				if ((TileMap[i][j] == 'h')) s_map.setTextureRect(IntRect(128, 0, 32, 32)); //��������� ��������


				s_map.setPosition(j * 32, i * 32);//�� ���� ����������� ����������, ��������� � �����. �� ���� ������ ������� �� ��� �������. ���� ������, �� ��� ����� ���������� � ����� �������� 32*32 � �� ������ ���� �������

				window.draw(s_map);//������ ���������� �� �����
			}
		std::ostringstream playerHealtString, gameTimeString;
		
		playerHealtString << p.health; gameTimeString << gameTime;		//��������� ������
		text.setString("��������: " + playerHealtString.str() + "\n����� ����: " + gameTimeString.str());
		text.setPosition(view.getCenter().x - 165, view.getCenter().y - 200);
		window.draw(text);

		window.draw(p.sprite);
		
		window.display();
	}

	return 0;
}
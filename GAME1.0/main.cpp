#include <SFML/Graphics.hpp>
#include "map.h"
#include "view.h"
using namespace sf;

class Player
{
private: float x, y = 0;
public:
	float w, h, dx, dy, speed = 0;
	int dir = 0;
	String File;
	Image image;
	Texture texture;
	Sprite sprite;
	Player(String F, int X, int Y, float W, float H)
	{
		File = F; // »м€ файла + расширение
		w = W; h = H; //¬ысота и ширина
		image.loadFromFile("images/" + File);
		image.createMaskFromColor(Color(41, 33, 59)); 
		texture.loadFromImage(image);
		sprite.setTexture(texture); //«аливаем спрайт текстурой
		x = X; y = Y; // оординаты по€влени€ спрайта
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

	}
		float getPlayercoordinateX()
		{
			return x;
		}
		float getPlayercoordinateY()
		{
			return y;
		}
	
};

int main()
{

	RenderWindow window(sf::VideoMode(640, 480), "Lesson 1. kychka-pc.ru");
	view.reset(FloatRect(0, 0, 640, 480));

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
			p.sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 288, 96, 96));
			
		}

		if ((Keyboard::isKeyPressed(Keyboard::Down)))
		{
			p.dir = 2; p.speed = 0.1;
			CurrentFrame += 0.005 * time;
			if (CurrentFrame > 3) CurrentFrame -= 3;
			p.sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 0, 96, 96));
			
		}
		getPlayercoordinateForView(p.getPlayercoordinateX(), p.getPlayercoordinateY());

		p.update(time);
		viewmap(time);
		window.setView(view);
		window.clear();
		//–исуем карту
		for (int i = 0; i < HEIGHT_MAP; i++)
			for (int j = 0; j < WIDTH_MAP; j++)
			{
				if (TileMap[i][j] == ' ')  s_map.setTextureRect(IntRect(0, 0, 32, 32)); //если встретили символ пробел, то рисуем 1й квадратик
				if (TileMap[i][j] == 's')  s_map.setTextureRect(IntRect(32, 0, 32, 32));//если встретили символ s, то рисуем 2й квадратик
				if ((TileMap[i][j] == '0')) s_map.setTextureRect(IntRect(64, 0, 32, 32));//если встретили символ 0, то рисуем 3й квадратик


				s_map.setPosition(j * 32, i * 32);//по сути раскидывает квадратики, превраща€ в карту. то есть задает каждому из них позицию. если убрать, то вс€ карта нарисуетс€ в одном квадрате 32*32 и мы увидим один квадрат

				window.draw(s_map);//рисуем квадратики на экран
			}
		window.draw(p.sprite);
		window.display();
	}

	return 0;
}
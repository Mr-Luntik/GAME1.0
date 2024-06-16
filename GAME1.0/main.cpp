#include <SFML/Graphics.hpp>

using namespace sf;

class Player
{
public:
	float x, y, w, h, dx, dy, speed = 0;
	int dir = 0;
	String File;
	Image image;
	Texture texture;
	Sprite sprite;
	Player(String F, int X, int Y, float W, float H)
	{
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
	}
};

int main()
{
	RenderWindow window(sf::VideoMode(640, 480), "Lesson 1. kychka-pc.ru");
	Image heroimog;
	heroimog.loadFromFile("./images/hero.png");


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
		
		if ((Keyboard::isKeyPressed(Keyboard::Left) || (Keyboard::isKeyPressed(Keyboard::A))))
		{
			p.dir = 1; p.speed = 0.1;
			CurrentFrame += 0.005 * time;
			if (CurrentFrame > 3) CurrentFrame -= 3;
			p.sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 96, 96, 96));
		}

		if ((Keyboard::isKeyPressed(Keyboard::Right) || (Keyboard::isKeyPressed(Keyboard::D))))
		{
			p.dir = 0; p.speed = 0.1;
			CurrentFrame += 0.005 * time;
			if (CurrentFrame > 3) CurrentFrame -= 3;
			p.sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 192, 96, 96));
		}

		if ((Keyboard::isKeyPressed(Keyboard::Up) || (Keyboard::isKeyPressed(Keyboard::W))))
		{
			p.dir = 3; p.speed = 0.1;
			CurrentFrame += 0.005 * time;
			if (CurrentFrame > 3) CurrentFrame -= 3;
			p.sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 288, 96, 96));
		}

		if ((Keyboard::isKeyPressed(Keyboard::Down) || (Keyboard::isKeyPressed(Keyboard::S))))
		{
			p.dir = 2; p.speed = 0.1;
			CurrentFrame += 0.005 * time;
			if (CurrentFrame > 3) CurrentFrame -= 3;
			p.sprite.setTextureRect(IntRect(96 * int(CurrentFrame), 0, 96, 96));
		}

		p.update(time);


		window.clear();
		window.draw(p.sprite);
		window.display();
	}

	return 0;
}
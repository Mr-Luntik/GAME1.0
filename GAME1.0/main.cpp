#include <SFML/Graphics.hpp>
#include "map.h"
#include "view.h"
#include <iostream>
#include <sstream>
#include "mission.h"
using namespace sf;
using namespace std;

class Player
{

public:
	float w, h, dx, dy, speed = 0, x, y = 0;
	int dir, playerScore, health;
	bool life, isMove, isSelect;
	String File;
	Image image;
	Texture texture;
	Sprite sprite;

	Player(String F, int X, int Y, float W, float H)
	{
		dir = 0; speed = 0; playerScore = 0, health = 100, dx = 0, dy = 0;
		life = true;
		isMove = false;
		isSelect = false;
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

	RenderWindow window(sf::VideoMode(1366, 768), "Lesson 1. kychka-pc.ru"/*, Style::Fullscreen*/);
	view.reset(FloatRect(0, 0, 640, 480));

	Font font;//шрифт 
	font.loadFromFile("CyrilicOld.ttf");//передаем нашему шрифту файл шрифта
	Text text("", font, 20);//создаем объект текст. закидываем в объект текст строку, шрифт, размер шрифта(в пикселях);//сам объект текст (не строка)
	text.setFillColor(Color::Black);//покрасили текст в красный. если убрать эту строку, то по умолчанию он белый
	text.setStyle(sf::Text::Bold | sf::Text::Underlined);

	Image heroimog;
	heroimog.loadFromFile("./images/hero.png");

	Image map_image;
	map_image.loadFromFile("./images/map.png");
	Texture map;
	map.loadFromImage(map_image);
	Sprite s_map;
	s_map.setTexture(map);

	Image quest_image;
	quest_image.loadFromFile("images/missionbg.jpg");
	quest_image.createMaskFromColor(Color(0, 0, 0));
	Texture quest_texture;
	quest_texture.loadFromImage(quest_image);
	Sprite s_quest;
	s_quest.setTexture(quest_texture);
	s_quest.setTextureRect(IntRect(0, 0, 340, 510));  //приведение типов, размеры картинки исходные
	s_quest.setScale(0.6f, 0.6f);//чуть уменьшили картинку, => размер стал меньше

	Player p("hero.png", 250, 250, 96.0, 96.0);

	bool showMissionText = true;

	float CurrentFrame = 0;
	Clock clock;
	Clock gameTimeClock;
	int gameTime = 0;
	bool isMove = false;
	float dX = 0;
	float dY = 0;
	int tempX = 0;
	int tempY = 0;
	float distance = 0;

	int createObjectForMapTimer = 0;

	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds();
		if (p.life) gameTime = gameTimeClock.getElapsedTime().asSeconds();//игровое время в секундах идёт вперед, пока жив игрок, перезагружать как time его не надо. оно не обновляет логику игры
		clock.restart();
		time = time / 800;

		Vector2i pixelPos = Mouse::getPosition(window);
		Vector2f pos = window.mapPixelToCoords(pixelPos);

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == Event::MouseButtonPressed)//если нажата клавиша мыши
				if (event.key.code == Mouse::Left) {//а именно левая
					if (p.sprite.getGlobalBounds().contains(pos.x, pos.y))//и при этом координата курсора попадает в спрайт
					{
						p.sprite.setColor(Color::Green);//красим спрайт в зеленый,тем самым говоря игроку,что он выбрал персонажа и может сделать ход
						p.isSelect = true;
					}
				}
			

			if (p.isSelect)//если выбрали объект
				if (event.type == Event::MouseButtonPressed)//если нажата клавиша мыши
					if (event.key.code == Mouse::Right) {//а именно правая
						p.isMove = true;//то начинаем движение
						p.isSelect = false;//объект уже не выбран
						p.sprite.setColor(Color::White);//возвращаем обычный цвет спрайту
						tempX = pos.x;//забираем координату нажатия курсора Х
						tempY = pos.y;//и Y

					}
		}


		if (p.isMove) {
			distance = sqrt((tempX - p.x) * (tempX - p.x) + (tempY - p.y) * (tempY - p.y));//считаем дистанцию (расстояние от точки А до точки Б). используя формулу длины вектора

			if (distance > 2) {//этим условием убираем дергание во время конечной позиции спрайта

				p.x += 0.1 * time * (tempX - p.x) / distance;//идем по иксу с помощью вектора нормали
				p.y += 0.1 * time * (tempY - p.y) / distance;//идем по игреку так же
			}
			else { p.isMove = false; std::cout << "priehali\n"; }//говорим что уже никуда не идем и выводим веселое сообщение в консоль

			//if (event.type == Event::KeyPressed)//событие нажатия клавиши
			//	if ((event.key.code == Keyboard::Tab)) {//если клавиша ТАБ
			//		switch (showMissionText) {//переключатель, реагирующий на логическую переменную showMissionText
			//		case true: {
			//			std::ostringstream playerHealthString;//строка здоровья игрока
			//			playerHealthString << p.health; //заносим в строку здоровье 
			//			std::ostringstream task;//строка текста миссии
			//			task << getTextMission(getCurrentMission(p.getPlayercoordinateX()));//вызывается функция getTextMission (она возвращает текст миссии), которая принимает в качестве аргумента функцию getCurrentMission(возвращающую номер миссии), а уже эта ф-ция принимает в качестве аргумента функцию p.getplayercoordinateX() (эта ф-ция возвращает Икс координату игрока)
			//			text.setString("Здоровье: " + playerHealthString.str() + "\n" + task.str());//задаем
			//			text.setPosition(view.getCenter().x + 125, view.getCenter().y - 130);//позиция всего этого текстового блока
			//			s_quest.setPosition(view.getCenter().x + 115, view.getCenter().y - 130);//позиция фона для блока
			//			showMissionText = false;//эта строка позволяет убрать все что мы вывели на экране
			//			break;//выходим , чтобы не выполнить условие "false" (которое ниже)
			//		}
			//		case false: {
			//			text.setString("");//если не нажата клавиша таб, то весь этот текст пустой
			//			showMissionText = true;// а эта строка позволяет снова нажать клавишу таб и получить вывод на экран
			//			break;
			//		}
			//		}
			//	}
		}

		if ((Keyboard::isKeyPressed(Keyboard::Escape)))
		{
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
			getPlayercoordinateForView(p.getPlayercoordinateX(), p.getPlayercoordinateY());
		}


		sf::Vector2i localPosition = Mouse::getPosition(window);

		if (localPosition.x < 3) { view.move(-0.2 * time, 0); }
		if (localPosition.x > window.getSize().x - 3) { view.move(0.2 * time, 0); }
		if (localPosition.y > window.getSize().y - 3) { view.move(0, 0.2 * time); }
		if (localPosition.y < 3) { view.move(0, -0.2 * time); }


		p.update(time);
		viewmap(time);
		window.setView(view);
		window.clear();
		//Рисуем карту
		if ((getCurrentMission(p.getPlayercoordinateX())) == 0) { //Если текущая миссия 0, то рисуем карту вот так
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
		}
		if ((getCurrentMission(p.getPlayercoordinateX())) >= 1) { //Если текущая миссия 1, то рисуем карту вот так
			for (int i = 0; i < HEIGHT_MAP; i++)
				for (int j = 0; j < WIDTH_MAP; j++)
				{
					if (TileMap[i][j] == ' ')  s_map.setTextureRect(IntRect(64, 0, 32, 32));//для примера поменял местами вывод спрайта для этого символа и..
					if (TileMap[i][j] == 's')  s_map.setTextureRect(IntRect(32, 0, 32, 32));
					if ((TileMap[i][j] == '0')) s_map.setTextureRect(IntRect(0, 0, 32, 32));//и для вот этого. логически-игровой смысл их остался таким же
					if ((TileMap[i][j] == 'f')) s_map.setTextureRect(IntRect(96, 0, 32, 32));
					if ((TileMap[i][j] == 'h')) s_map.setTextureRect(IntRect(128, 0, 32, 32));
					s_map.setPosition(j * 32, i * 32);
					window.draw(s_map);
				}
		}
		if (!showMissionText) {
			text.setPosition(view.getCenter().x + 125, view.getCenter().y - 130);//позиция всего этого текстового блока
			s_quest.setPosition(view.getCenter().x + 115, view.getCenter().y - 130);//позиция фона для блока			
			window.draw(s_quest); window.draw(text); //рисуем спрайт свитка (фон для текста миссии). а затем и текст. все это завязано на логическую переменную, которая меняет свое состояние от нажатия клавиши ТАБ
		}

		window.draw(p.sprite);

		window.display();
	}

	return 0;
}
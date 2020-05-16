#include "Scene.hh"
#include "MonsterFactory.hh"
#include "TileMap.hh"
#include "WindowManager.hh"
#include "Player.hh"
#include <unistd.h>

#include <iostream>

// static TileDescriptor level[] =
// {
// 	{ 0, 0, 1 },
// 	{ 31, 15, 2 },
// };

// static int level[] =
// {
// 	0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
// 	0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0,
// 	1, 1, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3,
// 	0, 1, 0, 0, 2, 0, 3, 3, 3, 0, 1, 1, 1, 0, 0, 0,
// 	0, 1, 1, 0, 3, 3, 3, 0, 0, 0, 1, 1, 1, 2, 0, 0,
// 	0, 0, 1, 0, 3, 0, 2, 2, 0, 0, 1, 1, 1, 1, 2, 0,
// 	2, 0, 1, 0, 3, 0, 2, 2, 2, 0, 1, 1, 1, 1, 1, 1,
// 	0, 0, 1, 0, 3, 2, 2, 2, 0, 0, 0, 0, 1, 1, 1, 1,
// };

int main()
{
	WindowManager::GetInstance().InitWindow("Mario Test", 512, 256, 60);
	sf::RenderWindow *win = WindowManager::GetInstance().GetWindow();
	TileMap m;
	sf::Clock Clock;

	if (!m.LoadTiles("res/tileset.png", sf::Vector2u(16, 16)))
	{
		std::cerr << "Can't load tileset!!" << std::endl;
		return -1;
	}
	m.SetBackground("res/background.png");
	m.InflateMapFromFile("res/map.dat");
	Player player;
	Comparator cmp(m);
	sf::Texture texture;
	if(!texture.loadFromFile(player.GetImage()))
	{
		std::cerr << "Can't load texture!!" << std::endl;
		return -1;
	}
	sf::Texture textureJump;
	if(!texture.loadFromFile(player.getJumpImage()))
	{
		std::cerr << "Can't load texture!!" << std::endl;
		return -1;
	}
	sf::Sprite sprite;
	sprite.setTexture(texture);
	sprite.setPosition(player.GetX(),player.GetY());
	sprite.setScale(2,2);
	player.setSprite(sprite);
	TimeManager::GetInstance().Start();
	player.setComparator(cmp);
	win->setFramerateLimit(60);
	int cpt = 1000;
	while (win->isOpen())
	{


		sf::Font MyFont;	
		if (!MyFont.loadFromFile("../assets/font/ASMAN.TTF"))
		{
			std::cerr << "Can't load font!!" << std::endl;
		}
		sf::Text Text;
		Text.setFont(MyFont);
		Text.setCharacterSize(20);
		Text.setFillColor(sf::Color::Black);
		sf::Event ev;

			while (win->pollEvent(ev))
		{
			if (ev.type == sf::Event::Closed)
			{
				win->close();
				return 0;
			}
			win->clear(sf::Color(255, 255, 255));

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				player.move(-0.4);
				player.changeOrientation(-2);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) 
			{
				player.move(0.4);
				player.changeOrientation(2);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			{
				player.jump();
			}
			while (player.fall() == 1)
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				{
					player.move(-0.2);
					player.changeOrientation(-2);
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) 
				{
					player.move(0.2);
					player.changeOrientation(2);
				}
				win->clear(sf::Color(255, 255, 255));
				win->draw(Text);	
				m.Draw();
				win->draw(player.getSprite());
				win->display();
				usleep(100);
				if (player.getSprite().getPosition().y >= 255)
				{
					return 0;
				}
			}
			player.fall();
			sprite.setTexture(texture);
			m.Draw();
	double framerate;
		TimeManager::GetInstance().Update();
		unsigned int tmp = TimeManager::GetInstance().GetElapsedTime();
		cpt -= tmp;
		std::cout << "Cpt : " << cpt << std::endl;
		std::cout << "tmp : " << tmp << std::endl;
		if(cpt <= 0)
		{
			framerate = 1 / (tmp * 0.001);
			Text.setString(std::to_string((int)framerate) + " FPS");
			cpt = 1000;
		}

			win->draw(Text);	
			win->draw(player.getSprite());
			win->display();

		}
	}
	return 0;
}

#include "Game.h"

Game::Game(const std::string& filename)
{
	sf::RenderWindow window(sf::VideoMode({ 300, 300 }), "Pong - SFML", sf::State::Windowed);

	while (window.isOpen())
	{
		if (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				window.close();
			}
		}
		window.clear();
		window.display();
	}

}

void Game::run()
{

}

#pragma once
#include <string>
#include <SFML/Graphics.hpp>
struct PlayerConfig { int W, H, CW, CH, FR, FG, FB, OR, OG, OB, OT; float S; };
struct BallConfig   { int SR, CR, FR, FG, FB, OR, OG, OB, OT, V; float S; };

class Game
{
public:
	sf::RenderWindow m_window;

	Game(const std::string& filename);
	void run();
};


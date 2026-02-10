#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "EntityManager.h"
struct PlayerConfig { int W, H, CW, CH, FR, FG, FB, OR, OG, OB, OT; float S; };
struct BallConfig   { int SR, CR, FR, FG, FB, OR, OG, OB, OT, V; float S; };

class Game
{
public:
	sf::RenderWindow m_window;
	EntityManager    m_entities;
	sf::Font         m_font;
	sf::Text         m_text;
	PlayerConfig     m_playerConfig;
	BallConfig		 m_ballConfig;
	sf::Clock		 m_deltaClock;
	int				 m_score[2] = { 0, 0 };
	int				 m_currentFrame = 0;
	bool			 m_paused = false;


	Game(const std::string& config);

	std::vector<std::shared_ptr<Entity>> players();
	std::shared_ptr<Entity> ball();
	void init(const std::string& config);
	void setPaused() { m_paused = false; }

	void spawnPlayers(bool isTwoPlayers);
	void spawnBall();

	void gameStart();

	void sMovement();
	void sUserInput();
	void sGUI();
	void sCollision();
	void sRender();

	void run();
};


#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "EntityManager.h"
struct PlayerConfig { int W, H, FR, FG, FB, OR, OG, OB, OT; float S; };
struct BallConfig   { int SR, FR, FG, FB, OR, OG, OB, OT, V; float S; };


class Game
{
public:
	enum GameState
	{
		MainMenu,
		Gameplay,
		Paused,
		GameOver
	};

	sf::RenderWindow m_window;
	sf::View m_view;
	EntityManager    m_entities;
	sf::Font         m_font;
	sf::Text         m_text;
	PlayerConfig     m_playerConfig;
	BallConfig		 m_ballConfig;
	sf::Clock		 m_deltaClock;
	int				 m_score[2]     = { 0, 0 };
	int				 m_currentFrame = 0;
	int              m_gameState    = Gameplay;
	float		     m_bSpeedMult   = 1.0f;
	bool             m_isTwoPlayer  = false;
	int				 m_scoreMax     =    3;

	Game(const std::string& config);

	std::vector<std::shared_ptr<Entity>> players();
	std::shared_ptr<Entity> ball();
	void init(const std::string& config);

	void spawnPlayers(bool isTwoPlayers);
	void spawnBall();

	void gameStart();
	void gameContinue();
	void gameOver();

	void sMovement();
	void sUserInput();
	void sGUI();
	void sCollision();
	void sRender();

	void run();
};


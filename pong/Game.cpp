#include "Game.h"
#include <iostream>
#include <fstream>

Game::Game(const std::string& config) :
	m_text(m_font, "default", 24)
{
	init(config);
	gameStart();
}

int randMinMax(int min, int max)
{
	// Garante que min <= max
	if (min > max) {
		std::swap(min, max);
	}

	// (max - min + 1) ex com [50, 100] -> rand() % 51, ou seja, vai de 0 a 50. após isso, somar o min (50), assim irá entre min, max.
	return min + (rand() % (max - min + 1));
}

void Game::init(const std::string& config)
{
	std::ifstream fin(config);
	// o que vai ler o config.txt
	std::string header;
	// o que vai identificar ( Window, Player, Ball )
	while (fin >> header)
	{
		if (header == "Window")
		{
			unsigned int wW, wH, fl;
			bool fs;
			fin >> wW;
			fin >> wH;
			fin >> fl;
			fin >> fs;
			sf::State state = sf::State::Windowed;
			if (fs == 1)
				state = sf::State::Fullscreen;
			m_window.create(sf::VideoMode({ wW, wH }), "Assignment 2", state);
			m_window.setFramerateLimit(fl);
			m_window.setKeyRepeatEnabled(false);

		}
		if (header == "Font")
		{
			std::string fontPath;
			int textSize;
			int fR, fG, fB;

			fin >> fontPath;
			fin >> textSize;
			fin >> fR;
			fin >> fG;
			fin >> fB;

			if (!m_font.openFromFile(fontPath)) // a mensagem de erro já existe na função openFromFile
				std::exit(-1);
		
			m_text.setCharacterSize(textSize);
			std::cout << fR << fB << fG << std::endl;
			m_text.setFillColor(sf::Color((uint8_t)fR, (uint8_t)fB, (uint8_t)fG));
		}
		if (header == "Player")
		{
			PlayerConfig temp{};
			fin >> temp.W;
			fin >> temp.H;
			fin >> temp.CW;
			fin >> temp.CH;
			fin >> temp.FR;
			fin >> temp.FG;
			fin >> temp.FB;
			fin >> temp.OR;
			fin >> temp.OG;
			fin >> temp.OB;
			fin >> temp.OT;
			fin >> temp.S;
			m_playerConfig = temp;
		}
		if (header == "Ball")
		{
			BallConfig temp{};
			fin >> temp.SR;
			fin >> temp.CR;
			fin >> temp.FR;
			fin >> temp.FG;
			fin >> temp.FB;
			fin >> temp.OR;
			fin >> temp.OG;
			fin >> temp.OB;
			fin >> temp.OT;
			fin >> temp.V;
			fin >> temp.S;
			m_ballConfig = temp;
		}
	}
}

void Game::spawnPlayers(bool isTwoPlayers)
{
	auto pC = m_playerConfig;
	
	std::shared_ptr<Entity> players[] = { m_entities.addEntity("player"), m_entities.addEntity("player") };
	
	Vec2f player1Pos(200.0f, m_window.getSize().y / 2.0f);
	Vec2f player2Pos(m_window.getSize().x - player1Pos.x, m_window.getSize().y - player1Pos.y);

	players[0]->add<CTransform>(player1Pos, Vec2f(0.0f, 0.0f), 0.0f);
	players[0]->add<CInput>();
	
	players[1]->add<CTransform>(player2Pos, Vec2f(0.0f, 0.0f), 0.0f);
	if (isTwoPlayers)
		players[1]->add<CInput>();

	for (auto p : players)
	{
		Vec2f size = { (float)pC.W, (float)pC.H };
		sf::Color fill((uint8_t)pC.FR, (uint8_t)pC.FG, (uint8_t)pC.FB);
		sf::Color out ((uint8_t)pC.OR, (uint8_t)pC.OG, (uint8_t)pC.OB);
		p->add<CRectangle>(size, fill, out, (float)pC.OT);
	}

}

void Game::spawnBall()
{
	auto bC = m_ballConfig;
	auto b = m_entities.addEntity("ball");


	Vec2f center(m_window.getSize().x / 2.0f, m_window.getSize().y / 2.0f);

	// A bola pode começar voando em quatro direções ( graus: 45, 135, 225, 315 )
	// para tanto, usa-se de vetores (1,1) , (-1,1), (1,-1), (-1,1), correspondente aos ângulos.
	srand((unsigned int)time(0)); // garante a aleatoridade do rand()
	
	Vec2f initialVelocity((float)randMinMax(0, 1), (float)randMinMax(0,1));
	initialVelocity.x = initialVelocity.x == 0.0f ? -1.0f : 1.0f;
	initialVelocity.y = initialVelocity.y == 0.0f ? -1.0f : 1.0f;

	initialVelocity.normalize(); // garante que a hiponetusa tenha tamanho 1.

	b->add<CTransform>(center, initialVelocity * bC.S, 0.0f);
	sf::Color fill((uint8_t)bC.FR, (uint8_t)bC.FG, (uint8_t)bC.FB);
	sf::Color out((uint8_t)bC.OR, (uint8_t)bC.OG, (uint8_t)bC.OB);
	b->add<CCircle>((float)bC.SR, (size_t)bC.V, fill, out,(float)bC.OT);
	b->get<CCircle>().shape.setPosition(center);
}

void Game::gameStart()
{
	m_gameState = Gameplay;

	if (!players().empty() && ball()->isAlive())
	{
		for (auto p : players())
		{
			p->destroy();
		}
		ball()->destroy();
	}

	spawnPlayers(false); // true => two players
	spawnBall();
}

void Game::gameOver()
{
	m_gameState = Game::GameOver;

	ball()->destroy();
	for (auto p : players())
	{
		p->destroy();
	}
}

void Game::sMovement()
{
	for (auto& p : players())
	{
		auto& input = p->get<CInput>();
		auto& rect = p->get<CRectangle>();
		auto& transform = p->get<CTransform>();
		if (input.exits) // se é player
		{
			float movement = (float)(input.down - input.up) * m_playerConfig.S;
			
			if (movement < 0) // para cima
			{
				if (rect.top().y + movement < 0) // se passar do topo da tela
				{
					transform.pos = Vec2f(transform.pos.x, rect.shape.getSize().y / 2);
					movement = 0;
				}
			}
			else // para baixo
			{
				if (rect.bottom().y + movement > m_window.getSize().y) // se passar do fim da tela
				{
					transform.pos = Vec2f(transform.pos.x, m_window.getSize().y - rect.shape.getSize().y / 2);
					movement = 0;
				}
			}
			transform.velocity.y = movement;
		}
		else // se é bot
		{    
			auto ballT = ball()->get<CTransform>();
			if (ballT.pos.y > transform.pos.y) // se a bola tiver para baixo do bot
			{
				if (std::abs(ballT.pos.y - transform.pos.y) > m_playerConfig.S)
				{
					transform.velocity.y = m_playerConfig.S;
				}
				else
				{
					transform.velocity.y = std::abs(ballT.pos.y - transform.pos.y);
				}
			}
			else if (ballT.pos.y < transform.pos.y) // se tiver para cima
			{
				if (std::abs(ballT.pos.y - transform.pos.y) > m_playerConfig.S)
				{
					transform.velocity.y = -m_playerConfig.S;
				}
				else
				{
					transform.velocity.y = -std::abs(ballT.pos.y - transform.pos.y);
				}
			}
			else
			{
				transform.velocity.y = 0;
			}
		}

		transform.pos += transform.velocity;
	}
	auto& ballT = ball()->get<CTransform>();
	ballT.pos += ballT.velocity;
}

void Game::sUserInput()
{
	while (auto event = m_window.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
		{
			std::exit(0);
		}

		if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
		{
			if (keyPressed->scancode == sf::Keyboard::Scancode::P)
				m_gameState = m_gameState == Gameplay ? Paused : Gameplay;
		}

		if (m_gameState != Gameplay)
			return;

		/*
			front(): pega o primeiro do vector.
			back() : pega o útlimo.
		*/
		std::shared_ptr<Entity> playerOne = players().front();
		std::shared_ptr<Entity> playerTwo;
		if (players().back()->get<CInput>().exits)
			playerTwo = players().back();
		else
			playerTwo = playerOne;

		if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
		{
			if (keyPressed->scancode == sf::Keyboard::Scancode::W)
				playerOne->get<CInput>().up = true;
			if (keyPressed->scancode == sf::Keyboard::Scancode::S)
				playerOne->get<CInput>().down = true;
			if (keyPressed->scancode == sf::Keyboard::Scancode::Up)
				playerTwo->get<CInput>().up = true;
			if (keyPressed->scancode == sf::Keyboard::Scancode::Down)
				playerTwo->get<CInput>().down = true;
		}
		if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>())
		{
			if (keyReleased->scancode == sf::Keyboard::Scancode::W)
				playerOne->get<CInput>().up = false;
			if (keyReleased->scancode == sf::Keyboard::Scancode::S)
				playerOne->get<CInput>().down = false;
			if (keyReleased->scancode == sf::Keyboard::Scancode::Up)
				playerTwo->get<CInput>().up = false;
			if (keyReleased->scancode == sf::Keyboard::Scancode::Down)
				playerTwo->get<CInput>().down = false;
		}
	}
}

void Game::sGUI()
{
}

void Game::sCollision()
{
	auto& bTransform = ball()->get<CTransform>();
	auto& bShape = ball()->get<CCircle>();
	if (bShape.top().y < 0 || bShape.bottom().y > m_window.getSize().y)
	{
		bTransform.velocity.y *= -1.0f;
	}
	if (bShape.left().x < 0)
	{

		m_score[1]++;
		if (m_score[1] >= 3)
			gameOver();
		else
			gameStart();
	}
	else if (bShape.right().x > m_window.getSize().x)
	{

		m_score[0]++;
		if (m_score[0] >= 3)
			gameOver();
		else
			gameStart();
	}

	for (auto p : players())
	{
		auto& pShape = p->get<CRectangle>();
		auto& pTransform = p->get<CTransform>();
		// colisão AABB
		if (bShape.right().x > pShape.left().x && 
			bShape.left().x < pShape.right().x && 
			bShape.top().y < pShape.bottom().y && 
			bShape.bottom().y > pShape.top().y )
		{ 
			Vec2f relPos = bTransform.pos - pTransform.pos;
			relPos.normalize();
			bTransform.velocity = relPos * m_ballConfig.S;
		}
	}
}



void Game::sRender()
{
	if (!m_window.isOpen()) { return; }

	m_window.clear();

	for (auto& e : m_entities.getEntities())
	{
		auto& t = e->get<CTransform>();
		if (e->get<CCircle>().exits)
		{
			auto& shape = e->get<CCircle>().shape;
			shape.setPosition(t.pos);
			m_window.draw(shape);
		}
		if (e->get<CRectangle>().exits)
		{
			auto& shape = e->get<CRectangle>().shape;
			shape.setPosition(t.pos);
			m_window.draw(shape);
		}
	}

	// usar o mesmo m_text para desenhar várias coisas.
	m_text.setString(std::to_string(m_score[0]));
	m_text.setPosition(Vec2f(m_window.getSize().x / 2 - 40, 20.0f));
	m_window.draw(m_text);

	m_text.setPosition(Vec2f(m_window.getSize().x / 2 + 40, 20.0f));
	m_text.setString(std::to_string(m_score[1]));
	m_window.draw(m_text);


	m_window.display();
}

std::vector<std::shared_ptr<Entity>> Game::players()
{
	return m_entities.getEntities("player");
}
std::shared_ptr<Entity> Game::ball()
{
	return m_entities.getEntities("ball").back();
}
void Game::run()
{
	while (true)
	{
		m_currentFrame++;
		m_entities.update();
		if (m_gameState == Game::Gameplay)
		{
			sCollision();
			sMovement();
		}
		sUserInput();

		sRender();
		sGUI();	
	}
}

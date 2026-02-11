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
	// para tanto, usarei de vetores (1,1) , (-1,1), (1,-1), (-1,1), correspondente aos ângulos.
	srand((unsigned int)time(0)); // garante a aleatoridade do rand()
	
	Vec2f initialVelocity((float)randMinMax(0, 1), (float)randMinMax(0,1));
	initialVelocity.x = initialVelocity.x == 0.0f ? -1.0f : 1.0f;
	initialVelocity.y = initialVelocity.y == 0.0f ? -1.0f : 1.0f;

	initialVelocity.normalize(); // garante que a hiponetusa tenha tamanho 1.

	b->add<CTransform>(center, initialVelocity * bC.S, 0.0f);
	sf::Color fill((uint8_t)bC.FR, (uint8_t)bC.FG, (uint8_t)bC.FB);
	sf::Color out((uint8_t)bC.OR, (uint8_t)bC.OG, (uint8_t)bC.OB);
	b->add<CCircle>((float)bC.SR, (size_t)bC.V, fill, out,(float)bC.OT);
}

void Game::gameStart()
{
	spawnPlayers(false); // true => two players
	spawnBall();
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
		/* 
			front(): pega o primeiro do vector.
			back() : pega o útlimo.
			Se tiver dois players, os inputs funcionarão como Up e Down para 
			o player dois e W e S para o player um.
			Se tiver apenas um player, playerOne = playerTwo,
			fazendo com que ambos W e S e Up e Down funcionem
			como input para o player.
		*/
		
		std::shared_ptr<Entity> playerOne = players().front();
		std::shared_ptr<Entity> playerTwo;
		if (players().back()->get<CInput>().exits)
			playerTwo = players().back();
		else
			playerTwo = playerOne;

		if (event->is<sf::Event::Closed>())
		{
			std::exit(0);
		}

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
		if (!m_paused)
		{
			sMovement();
		}
		sUserInput();
		sRender();
		sGUI();	
	}
}

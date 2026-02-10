#include "Game.h"

int main()
{
	Game g("config.txt");

	g.run();

	std::cin.get();
}
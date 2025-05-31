// SpaceWar.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <vector>
#include <ctime>
#include "GameState.h"
#include <iostream>

int main()
{
	std::srand(static_cast<unsigned>(time(NULL)));
	sf::RenderWindow gRenderWindow = sf::RenderWindow();
	try
	{
		GameState state(gRenderWindow);
		state.Initialize();
		while (state.IsWIndowOpen())
		{
			state.Tick();
		}
	}
	catch (sf::Exception exception)
	{
		std::cout << exception.what() << std::endl;
	}
	return 0;
}

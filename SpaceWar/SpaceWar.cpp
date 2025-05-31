// SpaceWar.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <vector>
#include <ctime>
#include "GameState.h"

int main()
{
	std::srand(static_cast<unsigned>(time(NULL)));

	GameState state;
	state.Initialize();
	while (state.IsWIndowOpen())
	{
		state.Tick();
	}
	return 0;
}

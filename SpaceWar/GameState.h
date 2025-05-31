#pragma once
#include "SFML/Graphics/RenderWindow.hpp"
#include "Game.h"
#include "PlayerState.h"

enum class EGameState
{
	MainMenu = 0,
	InGame,
	EndGame
};

class PlayerState;

class GameState
{
public:
	GameState(sf::RenderWindow& inWindow);
	~GameState() = default;

	// Create the window and initialize the main menu
	void Initialize();
	void Tick();

	// helpers and bindings
	void OnPlayerDead(PlayerState& deadPlayer);
	bool IsWIndowOpen() const { return mRenderWindow.isOpen(); }

private:
	void CreateWindow();
	
	// Main Menu stuff
	void InitMainMenu();
	void UpdateMainMenu();
	void RenderMainMenu();

	// Game stuff
	void InitGame();
	void UpdateGame();
	void RenderGame();
	
	// Game over screen
	void UpdateEndGame();
	void RenderEndGame();

	EGameState mCurrentState = EGameState::MainMenu;
	sf::RenderWindow& mRenderWindow;

	// Main menu stuff
	sf::Font mTitleFont;
	sf::Text mTitleText;
	sf::Font mStartFont;
	sf::Text mStartText;
	sf::Text mPlayersConnected;

	// End game stuff
	sf::Text mEndGameText;

	// game stuff
	std::vector<PlayerState*> mActivePlayerStates;
	std::vector<PlayerState*> mDeadPlayers;
	int mFPSCap = 120;
};


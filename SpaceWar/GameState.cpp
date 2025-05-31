#include "GameState.h"
#include "Ship.h"
#include "SFMLMath.hpp"

void GameState::Tick()
{
	int numJoystick = 0;
	for (int i = 0; i < sf::Joystick::Count; ++i)
	{
		if (sf::Joystick::isConnected(i))
		{
			++numJoystick;
		}
	}

	// Turns out poll events is crucial for the inputs to work properly,
	// as the input from the main menu was never getting reset without it.
	// This poll event should only handle window and main menu events
	// all game input is handled by the input system
	sf::Event inputEvent;
	while (mRenderWindow->pollEvent(inputEvent))
	{
		if (inputEvent.type == sf::Event::Closed)
		{
			if (mCurrentState == EGameState::InGame)
			{
				Game::Destroy();
			}
			mRenderWindow->close();
		}
		else if (inputEvent.type == sf::Event::JoystickButtonPressed)
		{
			if (mCurrentState == EGameState::MainMenu && numJoystick >= 1)
			{
				mCurrentState = EGameState::InGame;
				InitGame();
				return;
			}
			else if (mCurrentState == EGameState::EndGame)
			{
				mCurrentState = EGameState::MainMenu;
				InitMainMenu();
				return;
			}
		}
	}

	if (mCurrentState == EGameState::MainMenu)
	{
		UpdateMainMenu();
	}
	else if (mCurrentState == EGameState::InGame)
	{
		UpdateGame();
	}
	else if (mCurrentState == EGameState::EndGame)
	{
		UpdateEndGame();
	}
}

void GameState::OnPlayerDead(PlayerState& deadPlayer)
{
	for (auto it = mActivePlayerStates.begin(); it != mActivePlayerStates.end(); ++it)
	{
		if (*it == &deadPlayer)
		{
			mActivePlayerStates.erase(it);
			break;
		}
	}
	mDeadPlayers.push_back(&deadPlayer);
}

void GameState::Initialize()
{
	CreateWindow();

	mCurrentState = EGameState::MainMenu;
	InitMainMenu();
}

void GameState::InitMainMenu()
{
	if (mTitleFont.loadFromFile("Resources/fonts/TitleFont.otf"))
	{
		mTitleText.setFont(mTitleFont);
		// got the color value from Paint
		mTitleText.setFillColor(sf::Color(2, 153, 192));
		mTitleText.setCharacterSize(48);
		mTitleText.setPosition(sf::Vector2f(960, 480));
		mTitleText.setString("Space Wars Redux");
	}

	if (mStartFont.loadFromFile("Resources/fonts/GeneralFont.otf"))
	{
		mPlayersConnected.setFont(mStartFont);
		// got the color value from Paint
		mPlayersConnected.setFillColor(sf::Color(2, 153, 192));
		mPlayersConnected.setCharacterSize(24);
		mPlayersConnected.setPosition(sf::Vector2f(960, 680));

		mStartText.setFont(mStartFont);
		// got the color value from Paint
		mStartText.setFillColor(sf::Color(2, 153, 192));
		mStartText.setCharacterSize(24);
		mStartText.setPosition(sf::Vector2f(960, 720));
		mStartText.setString("");

		mEndGameText.setFont(mStartFont);
		// got the color value from Paint
		mEndGameText.setFillColor(sf::Color(2, 153, 192));
		mEndGameText.setCharacterSize(48);
		mEndGameText.setPosition(sf::Vector2f(960, 480));
		mEndGameText.setString("");
	}
}

void GameState::UpdateMainMenu()
{
	int numJoystick = 0;
	for (int i = 0; i < sf::Joystick::Count; ++i)
	{
		if (sf::Joystick::isConnected(i))
		{
			++numJoystick;
		}
	}

	std::stringstream playersConnectedStr;
	playersConnectedStr << numJoystick << " player(s) connected";
	mPlayersConnected.setString(playersConnectedStr.str());

	if (numJoystick >= 1)
	{
		mStartText.setString("Press any button to start");
	}

	RenderMainMenu();
}

void GameState::RenderMainMenu()
{
	mRenderWindow->clear(sf::Color::Black);

	sf::Vector2f bounds(mTitleText.getLocalBounds().width, mTitleText.getLocalBounds().height);
	mTitleText.setOrigin(bounds / 2.0f);
	mRenderWindow->draw(mTitleText);

	bounds = sf::Vector2f(mPlayersConnected.getLocalBounds().width, mPlayersConnected.getLocalBounds().height);
	mPlayersConnected.setOrigin(bounds / 2.0f);
	mRenderWindow->draw(mPlayersConnected);
	
	bounds = sf::Vector2f(mStartText.getLocalBounds().width, mStartText.getLocalBounds().height);
	mStartText.setOrigin(bounds / 2.0f);
	mRenderWindow->draw(mStartText);

	mRenderWindow->display();
}

void GameState::InitGame()
{
	Game* gameInstance = Game::Get(mRenderWindow->getSize());
	gameInstance->LoadGameTextures("Resources/assets");
	gameInstance->Init();

	InputSystem::CreatePlayerStates(mActivePlayerStates);

	// Create as many players as there are controllers
	for (size_t i = 0; i < mActivePlayerStates.size(); ++i)
	{
		std::stringstream shipName;
		shipName << "ship_" << RandomInRange<int>(1, 6);
		mActivePlayerStates[i]->Init(shipName.str());
		mActivePlayerStates[i]->BindOnPlayerDead(std::bind(&GameState::OnPlayerDead, this, std::placeholders::_1));
	}
}

void GameState::UpdateGame()
{
	if (mActivePlayerStates.size() <= 1)
	{
		mCurrentState = EGameState::EndGame;
		while (mDeadPlayers.size() > 0)
		{
			PlayerState* playerToDestroy = mDeadPlayers.back();
			mDeadPlayers.pop_back();
			delete playerToDestroy;
		}

		while (mActivePlayerStates.size() > 0)
		{
			PlayerState* playerToDestroy = mActivePlayerStates.back();
			mActivePlayerStates.pop_back();
			delete playerToDestroy;
		}

		Game::Destroy();
		return;
	}
	float deltaTime = 1.f / mFPSCap;

	// Updates all the systems and the game objects
	Game::Get(mRenderWindow->getSize())->Update(deltaTime);

	for (PlayerState* player : mActivePlayerStates)
	{
		player->ProcessInput(deltaTime);
		player->Update(deltaTime);
	}

	RenderGame();
}

void GameState::RenderGame()
{
	mRenderWindow->clear(sf::Color::Black);

	const std::vector<BaseGameObject*>& currentGameObjects = Game::GetAllObjects();
	for (BaseGameObject* object : currentGameObjects)
	{
		mRenderWindow->draw(object->GetSprite());
	}

	for (PlayerState* player : mActivePlayerStates)
	{
		player->RenderHud(*mRenderWindow);
	}

	mRenderWindow->display();
}

void GameState::UpdateEndGame()
{
	mEndGameText.setString("Game Over :(");
	mStartText.setString("Press any button to return to main menu");
	RenderEndGame();
}

void GameState::RenderEndGame()
{
	mRenderWindow->clear(sf::Color::Black);

	sf::Vector2f bounds;
	bounds = sf::Vector2f(mEndGameText.getLocalBounds().width, mEndGameText.getLocalBounds().height);
	mEndGameText.setOrigin(bounds / 2.0f);
	mRenderWindow->draw(mEndGameText);

	bounds = sf::Vector2f(mStartText.getLocalBounds().width, mStartText.getLocalBounds().height);
	mStartText.setOrigin(bounds / 2.0f);
	mRenderWindow->draw(mStartText);

	mRenderWindow->display();
}

void GameState::CreateWindow()
{
	mRenderWindow = new sf::RenderWindow(sf::VideoMode(1920, 1080), "SpaceWarsRedux", sf::Style::Close);
	mRenderWindow->setFramerateLimit(mFPSCap);
}

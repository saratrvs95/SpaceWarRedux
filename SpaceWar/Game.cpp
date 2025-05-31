#include "Game.h"
#include <iostream>
#include "SFMLMath.hpp"
#include "InputSystem.h"
#include "PhysicsSystem.h"
#include "CollisionSystem.h"
#include "Ship.h"
#include "Sun.h"
#include "Planet.h"
#include <sstream>

Game* Game::sInstance = nullptr;

Game::~Game()
{
	for (auto [_, texture] : mTextureMap)
	{
		delete texture;
		texture = nullptr;
	}
	mTextureMap.clear();
}

Game* Game::Get(sf::Vector2u windowSize)
{
	if (sInstance) return sInstance;
	sInstance = new Game(windowSize);
	return sInstance;
}

void Game::Destroy()
{
	DestroyAllObjects();
	PhysicsSystem::Destroy();
	InputSystem::Destroy();
	CollisionSystem::Destroy();
	delete sInstance;
	sInstance = nullptr;
}

const sf::Texture& Game::GetTexture(std::string textureName)
{
	return *sInstance->mTextureMap[textureName];
}

void Game::Init()
{
	InputSystem::Get();
	PhysicsSystem::Get();
	CollisionSystem::Get();

	sf::Vector2f sunPosition(960, 540);
	PhysicsGameObject* sun = static_cast<PhysicsGameObject*>(CreateNewObject<Sun>(*mTextureMap["sun_64"], sf::Vector2f(32, 32), sunPosition));
	sun->Init();
	PhysicsSystem::AddGravitationalObject(sun->GetPhysics());

	int randomNumberOfPlanets = RandomInRange<int>(1, 3);
	for (int i = 1; i <= randomNumberOfPlanets; ++i)
	{
		std::stringstream planetName;
		planetName << "planet_" << i;
		float randomDistanceFromSun = RandomInRange<float>(150.f, 250.f) * i;
		float randomRotation = RandomInRange<float>(0.f, 360.f);
		sf::Vector2f randomPosition = (sf::RotationToUnitVector(randomRotation) * randomDistanceFromSun) + sunPosition;

		PhysicsGameObject* planet = static_cast<PhysicsGameObject*>(CreateNewObject<Planet>(*mTextureMap[planetName.str()], sf::Vector2f(24, 24), randomPosition));
		planet->Init();
		PhysicsSystem::AddGravitationalObject(planet->GetPhysics());
	}
}

void Game::LoadGameTextures(const std::filesystem::path& path)
{
	// Idea for using the std::filesystem was from a video I saw for creating a
	// resource manager when I was working on my game engine a month ago

	for (auto& file : std::filesystem::directory_iterator(path))
	{
		std::filesystem::path currentPath = file.path();
		if (file.is_directory())
		{
			LoadGameTextures(currentPath);
		}
		else if (file.is_regular_file() && currentPath.extension() == ".png")
		{
			// this is an image file, so let's go ahead and load it in
			std::string filePath = currentPath.string();
			std::string fileName = currentPath.stem().string();
			mTextureMap.insert({ fileName, new sf::Texture() });
			if (!mTextureMap[fileName]->loadFromFile(filePath))
			{
				std::cout << "Failed to load asset " << fileName << " at location " << filePath << std::endl;
			}
		}
	}
}

void Game::Update(float deltaTime)
{
	DestroyAllMarkedObjects();

	// The objects may spawn other objects when input is processed or on update so we want to
	// cache the number of objects for this frame.
	// Next frame onwards, the new objects can be considered for input and update
	const size_t numObjects = mCurrentGameObjects.size();

	InputSystem::Update();

	PhysicsSystem::Update(deltaTime);
	PhysicsSystem::PostUpdate();

	for (size_t index = 0; index < numObjects; ++index)
	{
		BaseGameObject* object = mCurrentGameObjects[index];
		object->Update(deltaTime);
		if (PhysicsGameObject* physicsObject = dynamic_cast<PhysicsGameObject*>(object))
		{
			physicsObject->WrapPosition(sInstance->mWindowSize);
		}
	}

	CollisionSystem::Update(deltaTime);
	CollisionSystem::PostUpdate();
}

#pragma once
#include "SFML/Graphics.hpp"
#include <filesystem>
#include <string>
#include <unordered_map>
#include "GameObject.h"

template <typename T>
concept IsGameObjectType = requires (T object)
{
	static_cast<BaseGameObject&>(object);
};

class Ship;

class Game
{
public:
	~Game();

	// Create the game instance if not already existing
	// and return it
	static Game* Get(sf::Vector2u windowSize);

	// Destroys all game objects, systems and associated components
	// before deleting the game instance
	static void Destroy();

	template<typename T>
	requires IsGameObjectType<T>
	static BaseGameObject* CreateNewObject(const sf::Texture& texture, const sf::Vector2f& origin, sf::Vector2f startPosition)
	{
		sInstance->mCurrentGameObjects.emplace_back(new T(texture, origin, startPosition));
		return sInstance->mCurrentGameObjects.back();
	}

	// Marks an object for destruction, but does not remove it from the current objects list
	static void MarkObjectForDestruction(BaseGameObject* objectToDestroy)
	{
		for (auto it = sInstance->mCurrentGameObjects.begin(); it != sInstance->mCurrentGameObjects.end(); ++it)
		{
			if (*it == objectToDestroy)
			{
				sInstance->mObjectsToDestroy.push_back(objectToDestroy);
				break;
			}
		}
	}

	// Destroys all objects that are marked for destruction
	static void DestroyAllMarkedObjects()
	{
		while (sInstance->mObjectsToDestroy.size() > 0)
		{
			BaseGameObject* objectToDestroy = sInstance->mObjectsToDestroy.back();
			sInstance->mObjectsToDestroy.pop_back();
			DestroyObject(objectToDestroy);
		}
		sInstance->mObjectsToDestroy.clear();
	}

	// Destroys an object and removes it from the current objects list
	static void DestroyObject(BaseGameObject* objectToDestroy)
	{
		for (auto it = sInstance->mCurrentGameObjects.begin(); it != sInstance->mCurrentGameObjects.end(); ++it)
		{
			if (*it == objectToDestroy)
			{
				sInstance->mCurrentGameObjects.erase(it);
				delete objectToDestroy;
				break;
			}
		}
	}

	static void DestroyAllObjects()
	{
		while (sInstance->mCurrentGameObjects.size() > 0)
		{
			BaseGameObject* objectToDestroy = sInstance->mCurrentGameObjects.back();
			sInstance->mCurrentGameObjects.pop_back();
			delete objectToDestroy;
		}
		sInstance->mCurrentGameObjects.clear();
	}

	static const sf::Texture& GetTexture(std::string textureName);
	static std::vector<BaseGameObject*>& GetAllObjects() { return sInstance->mCurrentGameObjects; }

	// Loads all game texture assets (.png files) in the given directory
	void LoadGameTextures(const std::filesystem::path& path);

	// Initialize the systems and any essential game objects for the game
	void Init();

	void Update(float deltaTime);

private:
	Game(sf::Vector2u windowSize)
		:mWindowSize(windowSize)
	{}

	sf::Vector2u mWindowSize;
	std::unordered_map<std::string, sf::Texture*> mTextureMap;
	std::vector<BaseGameObject*> mCurrentGameObjects;
	std::vector<BaseGameObject*> mObjectsToDestroy;

	static Game* sInstance;
};

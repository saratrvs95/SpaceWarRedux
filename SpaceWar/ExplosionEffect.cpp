#include "ExplosionEffect.h"
#include "SFML/Graphics/Sprite.hpp"
#include "Game.h"

void ExplosionEffect::Init()
{
	sf::IntRect textureRect(sf::Vector2i(), sf::Vector2i(48, 48));
	mSprite.setTextureRect(textureRect);
	mFrameDuration = mEffectDuration / 7;
}

void ExplosionEffect::Update(float deltaTime)
{
	if (mFrameDuration <= 0.0f) return;

	mTimer += deltaTime;
	
	int offset = static_cast<int>(mTimer / mFrameDuration) * 48;
	sf::Vector2i texturePosition = sf::Vector2i(offset, 0);
	sf::IntRect textureRect(texturePosition, sf::Vector2i(48, 48));
	mSprite.setTextureRect(textureRect);

	if (mTimer >= mEffectDuration)
	{
		Game::MarkObjectForDestruction(this);
	}
}

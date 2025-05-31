#pragma once
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/System/Vector2.hpp"

/**
 * Purely meant as a representation of the data present in the PlayerState
 */
class PlayerHud
{
public:
	PlayerHud(sf::Vector2i screenPosition);
	~PlayerHud() = default;

	const sf::Sprite& GetHealthBar() const { return mHealthBar; }
	const sf::Sprite& GetEnergyBar() const { return mEnergyBar; }
	const sf::Text& GetRefillText() const { return mRefillPackText; }
	const sf::Sprite& GetMissile() const { return mMissile; }
	const sf::Text& GetMissileAmmoText() const { return mMissileAmmoText; }

	void UpdateSprite(int health, int maxHealth, int energy, int maxEnergy, int energyPacks, int missileAmmo);

private:
	sf::Vector2i mHudPosition;
	sf::Vector2i mVerticalSpacing;
	
	sf::Sprite mHealthBar;
	sf::Texture mHBTexture;

	sf::Sprite mEnergyBar;
	sf::Texture mEBTexture;

	sf::Sprite mMissile;
	sf::Texture mMissileTexture;

	sf::Font mRefillPackFont;
	sf::Text mRefillPackText;
	sf::Text mMissileAmmoText;
};


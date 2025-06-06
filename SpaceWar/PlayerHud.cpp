#include "PlayerHud.h"
#include "SFML/Graphics/Rect.hpp"
#include <sstream>

PlayerHud::PlayerHud(sf::Vector2i screenPosition)
	:mHBTexture("Resources/assets/health_bar.png"),
	mEBTexture("Resources/assets/power_bar.png"),
	mRefillPackFont("Resources/fonts/GeneralFont.otf"),
	mMissileTexture("Resources/assets/missile.png"),
	mHealthBar(mHBTexture),
	mEnergyBar(mEBTexture),
	mRefillPackText(mRefillPackFont),
	mMissile(mMissileTexture),
	mMissileAmmoText(mRefillPackFont)
{
	mHudPosition = screenPosition;
	mVerticalSpacing = sf::Vector2i(0, 48);
	sf::Vector2i horizontalSpacing = sf::Vector2i(12, 0);

	{
		mHealthBar.setPosition((sf::Vector2f)mHudPosition);
		mHealthBar.setOrigin(sf::Vector2f(24, 24));
		sf::IntRect textureRect(sf::Vector2i(), sf::Vector2i(48, 48));
		mHealthBar.setTextureRect(textureRect);
		mHealthBar.setScale(sf::Vector2f(4, 4));
	}

	{
		mEnergyBar.setPosition((sf::Vector2f)(mHudPosition + mVerticalSpacing));
		mEnergyBar.setOrigin(sf::Vector2f(24, 24));
		sf::IntRect textureRect(sf::Vector2i(), sf::Vector2i(48, 48));
		mEnergyBar.setTextureRect(textureRect);
		mEnergyBar.setScale(sf::Vector2f(4, 4));
	}

	{
		// got the color value from Paint
		mRefillPackText.setFillColor(sf::Color(2, 153, 192));
		mRefillPackText.setCharacterSize(24);
		mRefillPackText.setPosition((sf::Vector2f)(mHudPosition + mVerticalSpacing + sf::Vector2i(84, -12)));

		sf::Vector2f bounds = mRefillPackText.getLocalBounds().size;
		mRefillPackText.setOrigin(bounds / 2.0f);

		mRefillPackText.setString("");
	}

	{
		mMissile.setPosition((sf::Vector2f)(mHudPosition + sf::Vector2i(84, 0)));
		mMissile.setOrigin(sf::Vector2f(24, 24));
		sf::IntRect textureRect(sf::Vector2i(), sf::Vector2i(48, 48));
		mMissile.setTextureRect(textureRect);
		mMissile.setScale(sf::Vector2f(1.5f, 1.5f));

		// got the color value from Paint
		mMissileAmmoText.setFillColor(sf::Color(2, 153, 192));
		mMissileAmmoText.setCharacterSize(24);
		mMissileAmmoText.setPosition((sf::Vector2f)(mHudPosition + horizontalSpacing + sf::Vector2i(84, -12)));

		sf::Vector2f bounds = mMissileAmmoText.getLocalBounds().size;
		mMissileAmmoText.setOrigin(bounds / 2.0f);

		mMissileAmmoText.setString("");
	}
}

void PlayerHud::UpdateSprite(int health, int maxHealth, int energy, int maxEnergy, int energyPacks, int missileAmmo)
{
	{
		int offset = ((maxEnergy - energy) / 25) * 48;
		sf::Vector2i texturePosition = sf::Vector2i(offset, 0);
		sf::IntRect textureRect(texturePosition, sf::Vector2i(48, 48));
		mEnergyBar.setTextureRect(textureRect);

		std::stringstream text;
		text << "x" << energyPacks;
		mRefillPackText.setString(text.str());

		text.str("");
		text << "x" << missileAmmo;
		mMissileAmmoText.setString(text.str());
	}

	{
		int offset = ((maxHealth - health) / 25) * 48;
		sf::Vector2i texturePosition = sf::Vector2i(offset, 0);
		sf::IntRect textureRect(texturePosition, sf::Vector2i(48, 48));
		mHealthBar.setTextureRect(textureRect);
	}
}

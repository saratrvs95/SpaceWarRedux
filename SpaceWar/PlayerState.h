#pragma once
#include "MemberDelegate.h"
#include "GameObject.h"
#include "PlayerHud.h"
#include "InputSystem.h"

class Ship;

/**
 * Houses all the player abilities, cooldowns, and input functionality of the player
 */
class PlayerState
{
public:
	
	PlayerState(sf::Vector2i hudPosition);
	~PlayerState();

	bool operator==(const PlayerState& other) const
	{
		return mShip == other.mShip;
	}

	void Init(const std::string& shipName);
	void ProcessInput(float deltaTime);
	void Update(float deltaTime);
	void RenderHud(sf::RenderTarget& target);

	int GetScore() const { return mScore; }
	void AddScore() { ++mScore; }

	int GetHealth() const { return mHealth; }
	void TakeDamage(int damageAmount, PhysicsGameObject& collidingObject);

	void ActivatePowerPack(PhysicsGameObject& collidingObject);

	int GetCharge() const { return mEnergy + (mEnergyPack * mMaxEnergy); }
	void UseCharge(int chargeAmount);

	OneParamMemberDelegate(std::function<void(PlayerState&)>, OnPlayerDead, PlayerState)

private:
	int mScore = 0;
	const int mMaxHealth = 100;
	const int mMaxEnergy = 100;
	const int mMaxMissileAmmo = 5;
	int mEnergyPack = 5;
	int mHealth = mMaxHealth;
	int mEnergy = mMaxEnergy;
	int mMissileAmmo = 0;

	Ship* mShip = nullptr;
	PlayerHud mHud;

	InputComponent mInput;
	AbilityBlueprint mBoosterCooldown;
	AbilityBlueprint mPhaserShotCooldown;
	AbilityBlueprint mHyperdriveCooldown;
	AbilityBlueprint mMissileCooldown;
};


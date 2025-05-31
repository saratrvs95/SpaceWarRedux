#include "PlayerState.h"
#include "Game.h"
#include "Ship.h"
#include "PowerPack.h"
#include "ExplosionEffect.h"
#include <functional>
#include <iostream>

#if _WIN64
#include <windows.h>
#include <Xinput.h>
#endif

PlayerState::PlayerState(sf::Vector2i hudPosition)
	: mHud(hudPosition),
	mInput(),
	mBoosterCooldown(this, 1, -1.f),
	mPhaserShotCooldown(this, 10, 0.2f),
	mHyperdriveCooldown(this, 50, 2.0f),
	mMissileCooldown(this, 50, 1.f, 0.5f)
{
}

PlayerState::~PlayerState()
{
	if (mShip)
	{
		Game::DestroyObject(mShip);
	}
}

void PlayerState::Init(const std::string& shipName)
{
	mPhaserShotCooldown.xinputDeviceID = mInput.mControllerId;
	mBoosterCooldown.xinputDeviceID = mInput.mControllerId;
	mHyperdriveCooldown.xinputDeviceID = mInput.mControllerId;

	// find a random positions for all the ships near the edges
	sf::Vector2f position;
	do
	{
		position.x = (float)(std::rand() % 1920);
	} while (position.x > 720.f && position.x < 1200.f);
	do
	{
		position.y = (float)(std::rand() % 1080);
	} while (position.y > 405.f && position.y < 675.f);

	mShip = dynamic_cast<Ship*>(Game::CreateNewObject<Ship>(Game::GetTexture(shipName), sf::Vector2f(24, 24), position));
	mShip->Init();

	mShip->BindOnShipHit(std::bind(&PlayerState::TakeDamage, this, std::placeholders::_1, std::placeholders::_2));
	mShip->BindOnPowerPackHit(std::bind(&PlayerState::ActivatePowerPack, this, std::placeholders::_1));
	mShip->BindOnShipDowned(std::bind(&PlayerState::AddScore, this));
}

void PlayerState::Update(float deltaTime)
{
	mBoosterCooldown.Tick(deltaTime);
	mPhaserShotCooldown.Tick(deltaTime);
	mHyperdriveCooldown.Tick(deltaTime);

	mShip->SetCurrentRotation(mInput.mInputRotation);

	mHud.UpdateSprite(mHealth, mMaxHealth, mEnergy, mMaxEnergy, mEnergyPack, mMissileAmmo);
}

void PlayerState::ProcessInput(float deltaTime)
{
	// Handle input over here
#if _WIN64
	if (mInput.mButtonId != 0)
	{
		if (mInput.mButtonId & XINPUT_GAMEPAD_A && mBoosterCooldown.ActivateAbility())
		{
			UseCharge(mBoosterCooldown.GetAbilityCost());
			mShip->ActivateBoosters();
		}
		else if (mInput.mButtonId & XINPUT_GAMEPAD_B && mHyperdriveCooldown.ActivateAbility())
		{
			UseCharge(mHyperdriveCooldown.GetAbilityCost());
			mShip->EngageHyperdrive();
		}
		else if (mInput.mButtonId & XINPUT_GAMEPAD_X && mPhaserShotCooldown.ActivateAbility())
		{
			UseCharge(mPhaserShotCooldown.GetAbilityCost());
			mShip->FirePhaserShots();
		}
		else if (mInput.mButtonId & XINPUT_GAMEPAD_Y && mMissileCooldown.ActivateAbility())
		{
			if (mMissileAmmo > 0)
			{
				mShip->FireHomingMissile();
				--mMissileAmmo;
			}
		}
	}
#elif
	if (mInput.mButtonId != -1)
	{
		if (mInput.mButtonId == 0 && mBoosterCooldown.ActivateAbility())
		{
			std::cout << mInput.mButtonId << std::endl;
			UseCharge(mBoosterCooldown.GetAbilityCost());
			mShip->ActivateBoosters();
		}
		else if (mInput.mButtonId == 1 && mHyperdriveCooldown.ActivateAbility())
		{
			std::cout << mInput.mButtonId << std::endl;
			UseCharge(mHyperdriveCooldown.GetAbilityCost());
			mShip->EngageHyperdrive();
		}
		else if (mInput.mButtonId == 2 && mPhaserShotCooldown.ActivateAbility())
		{
			std::cout << mInput.mButtonId << std::endl;
			UseCharge(mPhaserShotCooldown.GetAbilityCost());
			mShip->FirePhaserShots();
		}
	}
#endif
}

void PlayerState::RenderHud(sf::RenderTarget& target)
{
	target.draw(mHud.GetHealthBar());
	target.draw(mHud.GetEnergyBar());
	target.draw(mHud.GetRefillText());
	target.draw(mHud.GetMissile());
	target.draw(mHud.GetMissileAmmoText());
}

void PlayerState::TakeDamage(int damageAmount, PhysicsGameObject& collidingObject)
{
	mHealth -= damageAmount;
	if (mHealth <= 0)
	{
		if (Ship* enemyShip = dynamic_cast<Ship*>(&collidingObject))
		{
			enemyShip->BroadcastOnShipDowned();
		}
		else if (Ship* enemyShip = dynamic_cast<Ship*>(collidingObject.GetParentObject()))
		{
			enemyShip->BroadcastOnShipDowned();
		}
		// handle player death here
		BroadcastOnPlayerDead(*this);

		BaseGameObject* explosion = static_cast<BaseGameObject*>(Game::CreateNewObject<ExplosionEffect>(Game::GetTexture("ship_explosion"), sf::Vector2f(24, 24), mShip->GetSprite().getPosition()));
		explosion->Init();

		mShip->UnbindOnShipHit(std::bind(&PlayerState::TakeDamage, this, std::placeholders::_1, std::placeholders::_2));
		mShip->UnbindOnShipDowned(std::bind(&PlayerState::AddScore, this));
		Game::MarkObjectForDestruction(mShip);
		mShip = nullptr;
	}
}

void PlayerState::ActivatePowerPack(PhysicsGameObject& collidingObject)
{
	PowerPack& powerPack = dynamic_cast<PowerPack&>(collidingObject);
	if (powerPack.GetType() == EPowerPackType::Health)
	{
		mHealth = std::min<int>(mHealth + PowerPack::GrantPowerPack(EPowerPackType::Health), mMaxHealth);
	}
	else if (powerPack.GetType() == EPowerPackType::Energy)
	{
		mEnergyPack += PowerPack::GrantPowerPack(EPowerPackType::Energy);
	}
	else if (powerPack.GetType() == EPowerPackType::Damage)
	{
		// grant missiles
		mMissileAmmo += std::min<int>(mMissileAmmo + PowerPack::GrantPowerPack(EPowerPackType::Damage), mMaxMissileAmmo);
	}
}

void PlayerState::UseCharge(int chargeAmount)
{
	mEnergy -= chargeAmount;
	if (mEnergy <= 0 && mEnergyPack > 0)
	{
		--mEnergyPack;
		mEnergy = mMaxEnergy;
	}
}

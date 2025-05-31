#include "Planet.h"
#include "SFMLMath.hpp"
#include "Game.h"

void Planet::Init()
{
	mPhysics.mMovable = false;
	mPhysics.mAffectedByGravity = false;
	mPhysics.mMaxSpeed = 0.0f;

	mPhysics.mAppliesGravity = true;
	mPhysics.mMass = 500.f;
	mPhysics.mGravityStrength = 1000.f;

	mPowerUpType = (EPowerPackType)RandomInRange<int>(1, (int)EPowerPackType::TypeMax);
	if (mPowerUpType != EPowerPackType::None)
	{
		mSpawnInterval = RandomInRange<float>(8.f, 15.f);
	}
}

void Planet::Update(float deltaTime)
{
	if (mPowerUpType == EPowerPackType::None) return;

	mTimer += deltaTime;
	if (mTimer >= mSpawnInterval)
	{
		mTimer = 0.0f;
		mSpawnInterval = RandomInRange<float>(8.f, 15.f);

		float randomDistance = RandomInRange<float>(50.f, 100.f);
		float randomRotation = RandomInRange<float>(0.f, 360.f);
		sf::Vector2f randomPosition = (sf::RotationToUnitVector(randomRotation) * randomDistance) + mPhysics.mPosition;

		const sf::Texture* textureToUse = nullptr;
		if (mPowerUpType == EPowerPackType::Health)
		{
			textureToUse = &Game::GetTexture("health_pack");
		}
		else if (mPowerUpType == EPowerPackType::Energy)
		{
			textureToUse = &Game::GetTexture("power_pack");
		}
		else if (mPowerUpType == EPowerPackType::Damage)
		{
			textureToUse = &Game::GetTexture("damage_pack");
		}
		
		if (textureToUse)
		{
			PowerPack* pack = static_cast<PowerPack*>(Game::CreateNewObject<PowerPack>(*textureToUse, sf::Vector2f(24, 24), randomPosition));
			pack->Init();
			pack->SetType(mPowerUpType);
			mPackList.push_back(pack);
		}
	}
}

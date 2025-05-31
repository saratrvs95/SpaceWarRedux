#pragma once
#include "GameObject.h"
#include "PowerPack.h"

class PowerPack;

class Planet : public PhysicsGameObject
{
public:
	Planet(const sf::Texture& texture, const sf::Vector2f& origin, sf::Vector2f startPosition)
		: PhysicsGameObject(texture, origin, startPosition)
	{}
	~Planet() = default;

	virtual void Init() override;
	virtual void Update(float deltaTime) override;
	virtual void OnCollisionCallback(PhysicsGameObject* collidingObject) override {}

private:
	float mSpawnInterval;
	float mTimer = 0.0f;
	EPowerPackType mPowerUpType = EPowerPackType::None;
	std::vector<PowerPack*> mPackList;
};


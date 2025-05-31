#pragma once
#include "GameObject.h"

class PhaserShots : public PhysicsGameObject
{
public:
	PhaserShots(const sf::Texture& texture, const sf::Vector2f& origin, sf::Vector2f startPosition)
		: PhysicsGameObject(texture, origin, startPosition)
	{}
	~PhaserShots();

	virtual void Init() override;
	virtual void Update(float deltaTime) override;
	virtual void OnCollisionCallback(PhysicsGameObject* collidingObject) override;

	static int mShotDamage;
};


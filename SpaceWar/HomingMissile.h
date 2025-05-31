#pragma once
#include "GameObject.h"

class HomingMissile : public PhysicsGameObject
{
public:
	HomingMissile(const sf::Texture& texture, const sf::Vector2f& origin, sf::Vector2f startPosition)
		: PhysicsGameObject(texture, origin, startPosition)
	{}

	~HomingMissile();

	virtual void Init() override;
	virtual void Update(float deltaTime) override;
	virtual void OnCollisionCallback(PhysicsGameObject* collidingObject) override;

	static int mMissileDamage;

private:
	bool SearchForTarget();

	PhysicsGameObject* mTarget = nullptr;
	float mAngularSpeed = 180.f;
};


#pragma once
#include "GameObject.h"

class Sun : public PhysicsGameObject
{
public:
	Sun(const sf::Texture& texture, const sf::Vector2f& origin, sf::Vector2f startPosition)
		: PhysicsGameObject(texture, origin, startPosition)
	{}
	~Sun();

	virtual void Init();
	virtual void Update(float deltaTime) {};
	virtual void OnCollisionCallback(PhysicsGameObject* collidingObject) override;
};


#pragma once
#include "GameObject.h"

/**
 * Explosion visual effect that should be spawned when two objects collide
 */
class ExplosionEffect : public BaseGameObject
{
public:
	ExplosionEffect(const sf::Texture& texture, const sf::Vector2f& origin, sf::Vector2f startPosition)
		: BaseGameObject(texture, origin, startPosition)
	{}
	~ExplosionEffect() = default;

	virtual void Init() override;
	virtual void Update(float deltaTime) override;

private:
	const float mEffectDuration = 0.85f;
	float mTimer = 0.0f;
	float mFrameDuration = 0.0f;
};


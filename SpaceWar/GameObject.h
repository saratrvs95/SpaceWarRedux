#pragma once
#include "SFML/Graphics/Sprite.hpp"
#include "PhysicsSystem.h"
#include "CollisionSystem.h"

class PlayerState;

class AbilityBlueprint
{
public:
	AbilityBlueprint(const PlayerState* owningPlayer, int abilityCost, float cooldownTime, float rumbleDuration = 0.2f)
		: mOwningPlayer(owningPlayer),
		mAbilityCost(abilityCost),
		mCooldown(cooldownTime),
		mTimer(0.0f),
		mRumbleDuration(rumbleDuration)
	{}

	~AbilityBlueprint();

	bool CanActivateAbility();

	int GetAbilityCost() const { return mAbilityCost; }

	bool ActivateAbility();

	void Tick(float deltaTime);

	uint8_t xinputDeviceID;
private:
	const PlayerState* mOwningPlayer;
	int mAbilityCost;
	const float mCooldown;
	float mTimer;

	const float mRumbleDuration = 0.2f;
	float mRumbleTimer = 0.0f;

};

/**
 * BaseGameObject: Can be used for anything that is static, doesn't need collision and has a sprite
 */
class BaseGameObject
{
public:
	BaseGameObject(const sf::Texture& texture, const sf::Vector2f& origin, sf::Vector2f startPosition)
	{
		mSprite.setTexture(texture);
		mSprite.setOrigin(origin);
		mSprite.setPosition(startPosition);
		mSprite.setRotation(0.0f);
		mSprite.setScale(sf::Vector2f(0.75f, 0.75f));
	}

	virtual ~BaseGameObject() = default;

	virtual void Init() = 0;
	virtual void Update(float deltaTime) = 0;

	virtual const sf::Sprite& GetSprite() const { return mSprite; }
	void SetRotation(float spriteRotation) { mSprite.setRotation(spriteRotation); }

protected:
	sf::Sprite mSprite;
};

/**
 * PhysicsGameObject: Slightly more functional game object. Can be used for all objects that need to
 * move around and register collisions.
 */
class PhysicsGameObject : public BaseGameObject
{
public:
	PhysicsGameObject(const sf::Texture& texture, const sf::Vector2f& origin, sf::Vector2f startPosition)
		: BaseGameObject(texture, origin, startPosition)
	{
		mPhysics.mPosition = startPosition;
	}

	virtual ~PhysicsGameObject() = default;

	virtual void WrapPosition(sf::Vector2u windowSize);
	virtual void OnCollisionCallback(PhysicsGameObject* collidingObject) = 0;
	
	PhysiscsComponent* GetPhysics() { return &mPhysics; }
	void SetVelocity(const sf::Vector2f& newVelocity) { mPhysics.mVelocity = newVelocity; }
	void IgnoreCollisionsFromObject(PhysicsGameObject* object) { mCollision->mIgnoredObjects.push_back(object); }
	PhysicsGameObject* GetParentObject() const { return mParentObject; }
	void SetParentObject(PhysicsGameObject* newParent) { mParentObject = newParent; }

protected:
	PhysiscsComponent mPhysics;
	CollisionComponent* mCollision = nullptr;
	PhysicsGameObject* mParentObject = nullptr;
};


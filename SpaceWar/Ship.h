#pragma once
#include "GameObject.h"
#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "MemberDelegate.h"

class PlayerState;

/**
 * Visual representation of the player state.
 * The player state class handles the input, the ship is passed to all the in-game systems
 * like physics and collisions. Sends events to the player state in the case of collisions with objects
 */
class Ship : public PhysicsGameObject
{
	friend PlayerState;

public:
	Ship(const sf::Texture& texture, const sf::Vector2f& origin, sf::Vector2f startPosition);
		
	~Ship();

	TwoParamMemberDelegate(std::function<void(int, PhysicsGameObject&)>, OnShipHit, int, PhysicsGameObject)
	OneParamMemberDelegate(std::function<void(PhysicsGameObject&)>, OnPowerPackHit, PhysicsGameObject)
	MemberDelegate(std::function<void()>, OnShipDowned)

	// Initializes the ships data, sprites, max move speed, max rotation speed, etc.
	virtual void Init() override;

	// Updates the ships rotation and sprite location to match the physics location
	virtual void Update(float deltaTime) override;
	virtual void OnCollisionCallback(PhysicsGameObject* collidingObject) override;

	void SetCurrentRotation(float rotation) { mInputRotation = rotation; }

private:
	// Functions housing all ability logic
	void ActivateBoosters();
	void FirePhaserShots();
	void EngageHyperdrive();
	void FireHomingMissile();

	void UpdateShipRotation();

	sf::Vector2f mForward;
	float mInputRotation;
	const float mAngularSpeed = 180.0f;
	const float mBoostStrength = 10000.f;
};


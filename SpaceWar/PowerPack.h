#pragma once
#include "GameObject.h"

enum class EPowerPackType : uint8_t
{
	None = 0,
	// Heals the ship for a certain amount
	Health,
	// Gives the ship a certain number of energy packs
	Energy,
	// Adds a fixed number of missiles to the ship
	Damage,
	TypeMax = 3
};

class PowerPack : public PhysicsGameObject
{
public:
	PowerPack(const sf::Texture& texture, const sf::Vector2f& origin, sf::Vector2f startPosition)
		: PhysicsGameObject(texture, origin, startPosition)
	{}

	~PowerPack();

	static int GrantPowerPack(EPowerPackType packType);

	virtual void Init() override;
	virtual void Update(float deltaTime) override;
	virtual void OnCollisionCallback(PhysicsGameObject* collidingObject) override;

	void SetType(EPowerPackType type) { mType = type; }
	EPowerPackType GetType() const { return mType; }

private:
	EPowerPackType mType;
};
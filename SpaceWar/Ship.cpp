#include "Ship.h"
#include <iostream>
#include "SFML/Window.hpp"
#include "SFMLMath.hpp"
#include "Game.h"
#include "PhaserShots.h"
#include "Sun.h"
#include "PlayerState.h"
#include "PowerPack.h"
#include "HomingMissile.h"

Ship::Ship(const sf::Texture& texture, const sf::Vector2f& origin, sf::Vector2f startPosition)
	: PhysicsGameObject(texture, origin, startPosition)
{}

Ship::~Ship()
{
	mCollision->MarkComponentForDelete();
}

void Ship::Init()
{
	mPhysics.mMovable = true;
	mPhysics.mAffectedByGravity = true;
	mPhysics.mMaxSpeed = 300.f;

	static sf::Vector2f defaultSize = sf::Vector2f(48, 48);
	sf::Vector2f spriteSize(defaultSize.x * mSprite.getScale().x, defaultSize.y * mSprite.getScale().y);
	sf::FloatRect collRect(mSprite.getPosition(), spriteSize);
	mCollision = new CollisionComponent(std::move(collRect), &mPhysics, this);
	mCollision->mMemberCallback = std::bind(&Ship::OnCollisionCallback, this, std::placeholders::_1);

	UpdateShipRotation();
}

void Ship::Update(float deltaTime)
{
	mSprite.setPosition(mPhysics.mPosition);
	mSprite.rotate(mInputRotation * mAngularSpeed * deltaTime);
	
	UpdateShipRotation();
}

void Ship::OnCollisionCallback(PhysicsGameObject* collidingObject)
{
	mCollision->ResetCollisionRegistered();
	if (PhaserShots* shot = dynamic_cast<PhaserShots*>(collidingObject))
	{
		BroadcastOnShipHit(PhaserShots::mShotDamage, *collidingObject);
	}
	else if (Sun* sunObject = dynamic_cast<Sun*>(collidingObject))
	{
		BroadcastOnShipHit(1000, *collidingObject);
	}
	if (HomingMissile* missile = dynamic_cast<HomingMissile*>(collidingObject))
	{
		BroadcastOnShipHit(HomingMissile::mMissileDamage, *collidingObject);
	}
	else if (PowerPack* powerPack = dynamic_cast<PowerPack*>(collidingObject))
	{
		BroadcastOnPowerPackHit(*powerPack);
	}
}

void Ship::ActivateBoosters()
{
	mPhysics.mNetForce += (mForward * mBoostStrength);
}

void Ship::FirePhaserShots()
{
	PhysicsGameObject* shot = static_cast<PhysicsGameObject*>(Game::CreateNewObject<PhaserShots>(Game::GetTexture("shot"), sf::Vector2f(24, 24), mPhysics.mPosition));
	shot->SetParentObject(this);

	shot->Init();
	shot->SetVelocity(mForward * 500.f);
	shot->SetRotation(mSprite.getRotation());
	shot->IgnoreCollisionsFromObject(this);
	IgnoreCollisionsFromObject(shot);
}

void Ship::EngageHyperdrive()
{
	mPhysics.mPosition.x = (float)(std::rand() % 1920);
	mPhysics.mPosition.y = (float)(std::rand() % 1080);
}

void Ship::FireHomingMissile()
{
	PhysicsGameObject* shot = static_cast<PhysicsGameObject*>(Game::CreateNewObject<HomingMissile>(Game::GetTexture("missile"), sf::Vector2f(24, 24), mPhysics.mPosition));
	shot->SetParentObject(this);

	shot->Init();
	shot->SetVelocity(mForward * 500.f);
	shot->SetRotation(mSprite.getRotation());
	shot->IgnoreCollisionsFromObject(this);
	IgnoreCollisionsFromObject(shot);
}

void Ship::UpdateShipRotation()
{
	float spriteRotation = mSprite.getRotation();
	mForward = sf::RotationToUnitVector(spriteRotation);
}

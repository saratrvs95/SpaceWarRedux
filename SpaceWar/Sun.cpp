#include "Sun.h"
#include <iostream>

Sun::~Sun()
{
	mCollision->MarkComponentForDelete();
}

void Sun::Init()
{
	mPhysics.mMovable = false;
	mPhysics.mAffectedByGravity = false;
	mPhysics.mMaxSpeed = 0.0f;

	mPhysics.mAppliesGravity = true;
	mPhysics.mMass = 5000.f;
	mPhysics.mGravityStrength = 1000.f;


	static sf::Vector2f defaultSize = sf::Vector2f(64, 64);
	sf::Vector2f spriteSize(defaultSize.x * mSprite.getScale().x, defaultSize.y * mSprite.getScale().y);
	sf::FloatRect collRect(mSprite.getPosition(), spriteSize);
	mCollision = new CollisionComponent(std::move(collRect), &mPhysics, this);
	mCollision->mMemberCallback = std::bind(&Sun::OnCollisionCallback, this, std::placeholders::_1);
}

void Sun::OnCollisionCallback(PhysicsGameObject* collidingObject)
{
	// Can the sun do anything when it collides with things?
	// Maybe add an easter egg here to where the sun grows bigger and bigger the more damage it takes?
	mCollision->ResetCollisionRegistered();
}

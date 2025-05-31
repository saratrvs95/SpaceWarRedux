#include "HomingMissile.h"
#include "Game.h"
#include "ExplosionEffect.h"
#include "Ship.h"
#include "SFMLMath.hpp"

int HomingMissile::mMissileDamage = 75;

HomingMissile::~HomingMissile()
{
	mCollision->MarkComponentForDelete();
}

void HomingMissile::Init()
{
	mPhysics.mMovable = true;
	mPhysics.mAffectedByGravity = false;
	mPhysics.mMaxSpeed = 400.0f;
	mPhysics.mMass = 0.0f;

	sf::IntRect spriteRect(sf::Vector2i(48, 0), sf::Vector2i(48, 48));
	mSprite.setTextureRect(spriteRect);

	static sf::Vector2f defaultSize = sf::Vector2f(20, 20);
	sf::Vector2f spriteSize(defaultSize.x * mSprite.getScale().x, defaultSize.y * mSprite.getScale().y);
	sf::FloatRect collRect(mSprite.getPosition(), spriteSize);
	mCollision = new CollisionComponent(std::move(collRect), &mPhysics, this);
	mCollision->mMemberCallback = std::bind(&HomingMissile::OnCollisionCallback, this, std::placeholders::_1);
}

void HomingMissile::Update(float deltaTime)
{
	mSprite.setPosition(mPhysics.mPosition);

	if (!mTarget)
	{
		if (!SearchForTarget()) return;
	}

	// Target found
	sf::Vector2f targetPosition = mTarget->GetPhysics()->mPosition;
	sf::Vector2f toTarget = targetPosition - mPhysics.mPosition;
	sf::Normalize(toTarget);
	
	const float spriteRotation = mSprite.getRotation().asDegrees();
	const sf::Vector2f forwardVector = sf::RotationToUnitVector(spriteRotation);
	float cosineTheta = sf::DotProduct(toTarget, forwardVector);
	float sineTheta = sqrt(1 - (cosineTheta * cosineTheta));
	mSprite.rotate(sf::degrees(sineTheta * mAngularSpeed * deltaTime));

	const float steeringStrength = 200.f;
	mPhysics.mVelocity += toTarget * steeringStrength;
}

void HomingMissile::OnCollisionCallback(PhysicsGameObject* collidingObject)
{
	BaseGameObject* explosion = static_cast<BaseGameObject*>(Game::CreateNewObject<ExplosionEffect>(Game::GetTexture("ship_explosion"), sf::Vector2f(24, 24), mPhysics.mPosition));
	explosion->Init();
	Game::MarkObjectForDestruction(this);
}

bool HomingMissile::SearchForTarget()
{
	std::vector<BaseGameObject*> gameObjects = Game::GetAllObjects();
	const float distanceThrehold = 200.f;

	auto predicate = [this, distanceThrehold](BaseGameObject* object) -> bool
		{
			if (Ship* ship = dynamic_cast<Ship*>(object))
			{
				std::vector<PhysicsGameObject*> ignoredObjectsList = mCollision->mIgnoredObjects;
				auto result = std::find(ignoredObjectsList.begin(), ignoredObjectsList.end(), ship);
				if (result == ignoredObjectsList.end())
				{
					return sf::SquaredDistance(mPhysics.mPosition, ship->GetPhysics()->mPosition) <= (distanceThrehold * distanceThrehold);
				}
			}

			return false;
		};

	auto result = std::find_if(gameObjects.begin(), gameObjects.end(), predicate);
	if (result == gameObjects.end()) return false;

	mTarget = static_cast<PhysicsGameObject*>(*result);
	return true;
}

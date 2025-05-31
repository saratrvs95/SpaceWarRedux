#include "PowerPack.h"
#include "Game.h"
#include "Sun.h"
#include "PhaserShots.h"
#include "ExplosionEffect.h"

PowerPack::~PowerPack()
{
	mCollision->MarkComponentForDelete();
}

int PowerPack::GrantPowerPack(EPowerPackType packType)
{
	int packAmount = 0;

	switch (packType)
	{
	case EPowerPackType::Health:
		packAmount = 100;
		break;
	case EPowerPackType::Energy:
		packAmount = 2;
		break;
	case EPowerPackType::Damage:
		packAmount = 2;
		break;
	default:
		break;
	}

	return packAmount;
}

void PowerPack::Init()
{
	mPhysics.mMovable = true;
	mPhysics.mAffectedByGravity = true;
	mPhysics.mAppliesGravity = false;
	mPhysics.mSatelliteBody = true;

	mSprite.setScale(sf::Vector2f(1, 1));
	sf::FloatRect collRect(mSprite.getPosition(), sf::Vector2f(19, 11));
	mCollision = new CollisionComponent(std::move(collRect), &mPhysics, this);
	mCollision->mMemberCallback = std::bind(&PowerPack::OnCollisionCallback, this, std::placeholders::_1);
}

void PowerPack::Update(float deltaTime)
{
	mSprite.setPosition(mPhysics.mPosition);
}

void PowerPack::OnCollisionCallback(PhysicsGameObject* collidingObject)
{
	if (Sun* sun = dynamic_cast<Sun*>(collidingObject))
	{
		BaseGameObject* explosion = static_cast<BaseGameObject*>(Game::CreateNewObject<ExplosionEffect>(Game::GetTexture("ship_explosion"), sf::Vector2f(24, 24), mPhysics.mPosition));
		explosion->Init();
	}
	else if (PhaserShots* shot = dynamic_cast<PhaserShots*>(collidingObject))
	{
		BaseGameObject* explosion = static_cast<BaseGameObject*>(Game::CreateNewObject<ExplosionEffect>(Game::GetTexture("ship_explosion"), sf::Vector2f(24, 24), mPhysics.mPosition));
		explosion->Init();
	}

	mCollision->ResetCollisionRegistered();
	Game::MarkObjectForDestruction(this);
}

#include "PhaserShots.h"
#include "Game.h"
#include "ExplosionEffect.h"

int PhaserShots::mShotDamage = 25;

PhaserShots::~PhaserShots()
{
	mCollision->MarkComponentForDelete();
}

void PhaserShots::Init()
{
	mPhysics.mMovable = true;
	mPhysics.mAffectedByGravity = false;
	mPhysics.mMaxSpeed = 500.0f;
	mPhysics.mMass = 0.0f;

	sf::IntRect spriteRect(sf::Vector2i(48, 0), sf::Vector2i(48, 48));
	mSprite.setTextureRect(spriteRect);
	
	static sf::Vector2f defaultSize = sf::Vector2f(20, 20);
	sf::Vector2f spriteSize(defaultSize.x * mSprite.getScale().x, defaultSize.y * mSprite.getScale().y);
	sf::FloatRect collRect(mSprite.getPosition(), spriteSize);
	mCollision = new CollisionComponent(std::move(collRect), &mPhysics, this);
	mCollision->mMemberCallback = std::bind(&PhaserShots::OnCollisionCallback, this, std::placeholders::_1);
}

void PhaserShots::Update(float deltaTime)
{
	mSprite.setPosition(mPhysics.mPosition);
}

void PhaserShots::OnCollisionCallback(PhysicsGameObject* collidingObject)
{
	BaseGameObject* explosion = static_cast<BaseGameObject*>(Game::CreateNewObject<ExplosionEffect>(Game::GetTexture("ship_explosion"), sf::Vector2f(24, 24), mPhysics.mPosition));
	explosion->Init();
	Game::MarkObjectForDestruction(this);
}

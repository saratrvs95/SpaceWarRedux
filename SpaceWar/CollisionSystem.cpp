#include "CollisionSystem.h"
#include "PhysicsSystem.h"
#include "GameObject.h"
#include "SFMLMath.hpp"

CollisionComponent::CollisionComponent(sf::FloatRect&& collisionRect, PhysiscsComponent* physicsComp, PhysicsGameObject* parent)
{
	mCollisionRect = new sf::FloatRect(collisionRect);
	mPhysics = physicsComp;
	mParentObject = parent;
	CollisionSystem::Get()->RegisterComponent(this);
}

CollisionComponent::~CollisionComponent()
{
	delete mCollisionRect;
	CollisionSystem::Get()->UnregisterComponent(this);
}

bool CollisionComponent::GetCollisionRegistered() const
{
	return mCollisionRegistered;
}

CollisionComponent* CollisionComponent::GetCollidingComponent() const
{
	return mCollisionRegistered ? mCollisionRegisteredWith : nullptr;
}

CollisionSystem* CollisionSystem::sInstance = nullptr;

CollisionSystem* CollisionSystem::Get()
{
	if (sInstance) return sInstance;
	sInstance = new CollisionSystem();
	return sInstance;
}

void CollisionSystem::Destroy()
{
	delete sInstance;
	sInstance = nullptr;
}

void CollisionSystem::Update(float deltaTime)
{
	size_t numComponents = sInstance->mRegisteredComponents.size();
	// Update the collision rect positions to align with the current positions of the physics components
	for (size_t i = 0; i < numComponents; ++i)
	{
		CollisionComponent* collisionComp = sInstance->mRegisteredComponents[i];
		PhysiscsComponent* physComp = collisionComp->mPhysics;
		collisionComp->mCollisionRect->left = physComp->mPosition.x - (collisionComp->mCollisionRect->width / 2);
		collisionComp->mCollisionRect->top = physComp->mPosition.y - (collisionComp->mCollisionRect->height / 2);
	}

	// Check each component against each other component
	// If an object has already registered a collision, don't check it again
	// If an object is already marked for delete, somehow, it shouldn't be registering collisions anyways
	for (size_t i = 0; i < numComponents; ++i)
	{
		CollisionComponent* currentComp = sInstance->mRegisteredComponents[i];
		if (currentComp->GetCollisionRegistered() || currentComp->IsMarkedForDelete()) continue;

		sf::Vector2f currentPos = currentComp->mPhysics->mPosition;

		for (size_t j = 0; j < numComponents; ++j)
		{
			// same object?
			if (j == i) continue;

			CollisionComponent* testComp = sInstance->mRegisteredComponents[j];
			if (testComp->GetCollisionRegistered() || currentComp->IsMarkedForDelete()) continue;

			// if the test component's parent object is in the ignore list, ignore collisions
			if (std::find(currentComp->mIgnoredObjects.begin(), currentComp->mIgnoredObjects.end(), testComp->mParentObject) != currentComp->mIgnoredObjects.end())
			{
				continue;
			}

			sf::Vector2f testPos = testComp->mPhysics->mPosition;

			// do preliminary distance checks, collisions cannot happen between objects that aren't close enough
			if (sf::SquaredDistance(testPos, currentPos) < (50 * 50))
			{
				// now do the shape overlap check
				if (currentComp->mCollisionRect->intersects(*testComp->mCollisionRect))
				{
					// register the collision
					currentComp->SetCollisionRegistered(testComp);
					testComp->SetCollisionRegistered(currentComp);
				}
			}
		}
	}
}

void CollisionSystem::PostUpdate()
{
	// now that all collisions have been registered, go through each component
	// and notify the parent that collisions have been registered and let
	// the parent handle what to do with it
	size_t numComponents = sInstance->mRegisteredComponents.size();
	for (size_t i = 0; i < numComponents; ++i)
	{
		CollisionComponent* currentComp = sInstance->mRegisteredComponents[i];
		if (!currentComp->GetCollisionRegistered() || currentComp->IsMarkedForDelete()) continue;

		CollisionComponent* collidingComp = currentComp->GetCollidingComponent();
		currentComp->mMemberCallback(collidingComp->mParentObject);
	}

	// wait for all the collision callbacks to go out and then do cleanup
	std::vector<CollisionComponent*> componentsToDelete;
	for (size_t i = 0; i < numComponents; ++i)
	{
		CollisionComponent* currentComp = sInstance->mRegisteredComponents[i];
		if (currentComp->IsMarkedForDelete())
		{
			componentsToDelete.push_back(currentComp);
		}
	}

	while (componentsToDelete.size() > 0)
	{
		CollisionComponent* component = componentsToDelete.back();
		// deleting the component removes it from the registered components list
		delete component;
		// only thing remaining is popping back from this temp list
		componentsToDelete.pop_back();
	}
}

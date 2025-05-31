#pragma once
#include "SFML/Graphics/Rect.hpp"
#include "IComponentRegistry.h"
#include <functional>

struct PhysiscsComponent;
class PhysicsGameObject;

/**
 * Component to store any and all collision related data.
 * Needs a collision rect, pointer to the parent object and its physics component (mostly for positional data)
 * Do not stack allocate this component, heap allocate instead.
 * Do not forget to assign the member callback in case a collision is registered.
 * Do not destroy the component on your own either, use MarkComponentForDelete instead.
 * The system internally takes care of deleting all components that are marked for delete.
 */
struct CollisionComponent
{
	CollisionComponent(sf::FloatRect&& collisionRect, PhysiscsComponent* physicsComp, PhysicsGameObject* parent);
	~CollisionComponent();

	sf::FloatRect* mCollisionRect;
	PhysiscsComponent* mPhysics;
	PhysicsGameObject* mParentObject;
	std::vector<PhysicsGameObject*> mIgnoredObjects;

	std::function<void(PhysicsGameObject*)> mMemberCallback;

	bool GetCollisionRegistered() const;
	CollisionComponent* GetCollidingComponent() const;
	bool IsMarkedForDelete() const { return mMarkedForDelete; }

	void SetCollisionRegistered(CollisionComponent* registeredWith) { mCollisionRegistered = true; mCollisionRegisteredWith = registeredWith; }
	void ResetCollisionRegistered() { mCollisionRegistered = false; mCollisionRegisteredWith = nullptr; }
	void MarkComponentForDelete() { mMarkedForDelete = true; }

private:
	bool mCollisionRegistered = false;
	CollisionComponent* mCollisionRegisteredWith = nullptr;
	bool mMarkedForDelete = false;
};

class CollisionSystem : public IComponentRegistry<CollisionComponent>
{
public:
	~CollisionSystem() = default;

	static CollisionSystem* Get();
	static void Destroy();

	// Checks for collisions on each registered component
	static void Update(float deltaTime);

	// Resolves the collisions by triggering the collision callback
	static void PostUpdate();

private:
	CollisionSystem() = default;

	static CollisionSystem* sInstance;
};


#pragma once
#include "IComponentRegistry.h"
#include "SFML/System/Vector2.hpp"

/**
 * Component that forms the core part of all PhysicsGameObject
 * The physics component houses the current position, velocity and
 * gravitational force of an object. Also stores intermediate values
 * like net force for the physics system so objects can apply a net force
 * outside of the gravitational forces acting on the object.
 */
struct PhysiscsComponent
{
	PhysiscsComponent();
	~PhysiscsComponent();

	bool mMovable = false;
	sf::Vector2f mNetForce;
	sf::Vector2f mVelocity;
	sf::Vector2f mPosition;
	float mMass = 1.0f;
	float mMaxSpeed = 300.f;

	bool mAffectedByGravity = false;
	bool mSatelliteBody = false;

	bool mAppliesGravity = false;
	float mGravityStrength = 0.0f;
};

class PhysicsSystem : public IComponentRegistry<PhysiscsComponent>
{
public:
	~PhysicsSystem() = default;

	static PhysicsSystem* Get();
	static void Destroy();

	// Adds an object that exerts gravitational force to a list so net gravity can be
	// calculated at the beginning each component's update
	static void AddGravitationalObject(PhysiscsComponent* gravitationalObject);

	// Updates the velocity of components based on the forces being applied on them
	// and then updates the position based on these velocities and delta time
	static void Update(float deltaTime);

	// Resets the net force on all objects that move for the next update
	static void PostUpdate();

private:
	PhysicsSystem() = default;

	// Helper functions
	static void ClampVelocity(PhysiscsComponent* component);
	static void CalculateNetForce(PhysiscsComponent* component, float deltaTime);
	static bool HandleSatelliteBody(PhysiscsComponent* component, float deltaTime);
	static sf::Vector2f CalculateOrbitalVelocity(const sf::Vector2f& centreOfGravity, float netGravityStrength, const PhysiscsComponent* orbitingComponent);

	std::vector<PhysiscsComponent*> mGravitationalObjects;

	static PhysicsSystem* sInstance;
};


#include "PhysicsSystem.h"
#include "SFMLMath.hpp"

PhysiscsComponent::PhysiscsComponent()
{
	PhysicsSystem::Get()->RegisterComponent(this);
}

PhysiscsComponent::~PhysiscsComponent()
{
	PhysicsSystem::Get()->UnregisterComponent(this);
}

PhysicsSystem* PhysicsSystem::sInstance = nullptr;

PhysicsSystem* PhysicsSystem::Get()
{
	if (sInstance) return sInstance;

	sInstance = new PhysicsSystem();
	return sInstance;
}

void PhysicsSystem::Destroy()
{
	delete sInstance;
	sInstance = nullptr;
}

void PhysicsSystem::AddGravitationalObject(PhysiscsComponent* gravitationalObject)
{
	if (!sInstance) return;
	if (!gravitationalObject || !gravitationalObject->mAppliesGravity) return;
	sInstance->mGravitationalObjects.push_back(gravitationalObject);
}

void PhysicsSystem::Update(float deltaTime)
{
	for (PhysiscsComponent* component : sInstance->mRegisteredComponents)
	{
		if (component->mMovable)
		{
			if (component->mMass > 0.0f)
			{
				if (HandleSatelliteBody(component, deltaTime)) continue;

				CalculateNetForce(component, deltaTime);

				// calculate the velocity based on the net force and add it to the existing velocity
				// a = F/m -> deltaV = a * deltaTime
				component->mVelocity += (component->mNetForce / component->mMass) * deltaTime;
			}

			// update the position
			ClampVelocity(component);
			component->mPosition += component->mVelocity * deltaTime;
		}
	}
}

void PhysicsSystem::ClampVelocity(PhysiscsComponent* component)
{
	if (sf::SquaredLength(component->mVelocity) > component->mMaxSpeed * component->mMaxSpeed)
	{
		Normalize(component->mVelocity);
		component->mVelocity *= component->mMaxSpeed;
	}
}

void PhysicsSystem::CalculateNetForce(PhysiscsComponent* component, float deltaTime)
{
	// calculate net gravitational influence on the component first
	if (component->mAffectedByGravity && sInstance->mGravitationalObjects.size() > 0)
	{
		sf::Vector2f netGravity;
		for (PhysiscsComponent* object : sInstance->mGravitationalObjects)
		{
			sf::Vector2f toObject = object->mPosition - component->mPosition;
			float length = sf::Length(toObject);
			sf::Normalize(toObject);
			float gravitationalForce = (object->mGravityStrength * object->mMass * component->mMass) / (length * length);
			netGravity += toObject * gravitationalForce;
		}

		component->mNetForce += netGravity;
	}
}

bool PhysicsSystem::HandleSatelliteBody(PhysiscsComponent* component, float deltaTime)
{
	if (component->mSatelliteBody)
	{
		sf::Vector2f centreOfGravity;
		float netGravitatinalStrength = 0.0f;
		for (PhysiscsComponent* object : sInstance->mGravitationalObjects)
		{
			centreOfGravity += object->mPosition;
			netGravitatinalStrength += object->mGravityStrength * object->mMass;
		}

		centreOfGravity /= (float)sInstance->mGravitationalObjects.size();
		component->mVelocity = CalculateOrbitalVelocity(centreOfGravity, netGravitatinalStrength, component);
		component->mPosition += component->mVelocity * deltaTime;
		return true;
	}

	return false;
}

void PhysicsSystem::PostUpdate()
{
	for (PhysiscsComponent* component : sInstance->mRegisteredComponents)
	{
		if (component->mMovable && component->mMass > 0.0f)
		{
			component->mNetForce = sf::Vector2f();
		}
	}
}

sf::Vector2f PhysicsSystem::CalculateOrbitalVelocity(const sf::Vector2f& centreOfGravity, float netGravityStrength, const PhysiscsComponent* orbitingComponent)
{
	sf::Vector2f toSatellite = orbitingComponent->mPosition - centreOfGravity;
	float orbitingDistance = sf::Length(toSatellite);
	float orbitingSpeed = sqrt(netGravityStrength / orbitingDistance);
	sf::Vector2f orbitingVelocity = sf::SafeNormalize(sf::Vector2f(toSatellite.y, -toSatellite.x)) * orbitingSpeed;
	return orbitingVelocity;
}

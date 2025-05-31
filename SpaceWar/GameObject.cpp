#include "GameObject.h"
#include "PlayerState.h"

#if _WIN64
#include <windows.h>
#include <Xinput.h>
#endif

AbilityBlueprint::~AbilityBlueprint()
{
#if _WIN64
	XINPUT_VIBRATION vibration(0, 0);
	XInputSetState(xinputDeviceID, &vibration);
#endif
}

bool AbilityBlueprint::CanActivateAbility()
{
	return (mOwningPlayer->GetCharge() > mAbilityCost && mTimer <= 0.0f);
}

bool AbilityBlueprint::ActivateAbility()
{
	if (CanActivateAbility())
	{
		mTimer = mCooldown;
#if _WIN64
		XINPUT_VIBRATION vibration(64000, 64000);
		XInputSetState(xinputDeviceID, &vibration);
#endif

		return true;
	}

	return false;
}

void AbilityBlueprint::Tick(float deltaTime)
{
	if (mTimer > 0.0f && mTimer <= mCooldown)
	{
		mTimer -= deltaTime;
	}
	
	mRumbleTimer += deltaTime;
	if (mRumbleTimer >= mRumbleDuration)
	{
		mRumbleTimer = 0.0f;
#if _WIN64
		XINPUT_VIBRATION vibration(0, 0);
		XInputSetState(xinputDeviceID, &vibration);
#endif
	}
}

void PhysicsGameObject::WrapPosition(sf::Vector2u windowSize)
{
	if (mPhysics.mPosition.x > windowSize.x)
	{
		mPhysics.mPosition.x = 0.0f;
	}
	else if (mPhysics.mPosition.x < 0.0f)
	{
		mPhysics.mPosition.x = (float)windowSize.x;
	}

	if (mPhysics.mPosition.y > windowSize.y)
	{
		mPhysics.mPosition.y = 0.0f;
	}
	else if (mPhysics.mPosition.y < 0.0f)
	{
		mPhysics.mPosition.y = (float)windowSize.y;
	}
}

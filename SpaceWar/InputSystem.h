#pragma once
#include <vector>
#include "IComponentRegistry.h"

class PlayerState;

struct InputComponent
{
	InputComponent();
	~InputComponent();

	uint8_t mControllerId = -1;
	float mInputRotation = 0.0f;
	int mButtonId = -1;
};

class InputSystem : public IComponentRegistry<InputComponent>
{
public:
	~InputSystem() = default;

	static InputSystem* Get();
	static void Destroy();
	/**
	 * Read inputs for the connected controllers and store the input
	 * in the resulting input components so the entity can do with them
	 * as it wishes
	 */
	static void Update();

	static void CreatePlayerStates(std::vector<PlayerState*>& outPlayerStates);

	virtual void RegisterComponent(InputComponent* component) override;

private:
	InputSystem() = default;

	static InputSystem* sInstance;
};


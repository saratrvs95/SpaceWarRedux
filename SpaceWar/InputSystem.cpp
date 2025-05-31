#include "InputSystem.h"
#include "SFML/Window.hpp"
#include "PlayerState.h"

#if _WIN64
#include <windows.h>
#include <Xinput.h>
#endif

InputComponent::InputComponent()
{
	InputSystem::Get()->RegisterComponent(this);
}

InputComponent::~InputComponent()
{
	InputSystem::Get()->UnregisterComponent(this);
}

InputSystem* InputSystem::sInstance = nullptr;

InputSystem* InputSystem::Get()
{
	if (sInstance) return sInstance;

	sInstance = new InputSystem();
	return sInstance;
}

void InputSystem::Destroy()
{
	delete sInstance;
	sInstance = nullptr;
}

void InputSystem::RegisterComponent(InputComponent* component)
{
	IComponentRegistry<InputComponent>::RegisterComponent(component);
#if _WIN64
	// idea here is to find the first connected controller (new controller)
	// new controller will always have an ID >= mRegisteredComponents.size() - 1
	// The reason behind going all the way to max user count is because it is
	// entirely possible for controllers to not be disconnected in order
	// For instance, if 3 controllers were connected to the PC at some point, it is possible that
	// controllers 0 and 1 have disconnected, so we don't want to be setting either of those as the
	// controller ID as rumble will not work. What we want is controller 2 that is still connected.
	// got the idea for this from https://learn.microsoft.com/en-us/windows/win32/xinput/getting-started-with-xinput
	DWORD dwResult;
	for (DWORD i = (DWORD)mRegisteredComponents.size() - 1; i < XUSER_MAX_COUNT; i++)
	{
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		// Simply get the state of the controller from XInput.
		dwResult = XInputGetState(i, &state);
		if (dwResult == ERROR_SUCCESS)
		{
			component->mControllerId = (uint8_t)(i);
			break;
		}
	}
#elif
	component->mControllerId = (uint8_t)(mRegisteredComponents.size() - 1);
#endif
}

void InputSystem::Update()
{
	for (InputComponent* component : sInstance->mRegisteredComponents)
	{
#if _WIN64
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		DWORD dwResult = XInputGetState(component->mControllerId, &state);
		if (dwResult == ERROR_SUCCESS)
		{
			component->mInputRotation = (state.Gamepad.bRightTrigger / 255.f);
			component->mInputRotation += (state.Gamepad.bLeftTrigger / -255.f);
			component->mButtonId = state.Gamepad.wButtons;
			//for (uint8_t index = 0; index < sf::Joystick::getButtonCount(component->mControllerId); ++index)
			//{
			//	if (sf::Joystick::isButtonPressed(component->mControllerId, index))
			//	{
			//		component->mButtonId = index;
			//		break;
			//	}
			//}
		}
#elif
		if (sf::Joystick::isConnected(component->mControllerId))
		{
			component->mInputRotation = (sf::Joystick::getAxisPosition(component->mControllerId, sf::Joystick::Axis::Z) / 100.f);
			component->mButtonId = -1;
			for (uint8_t index = 0; index < sf::Joystick::getButtonCount(component->mControllerId); ++index)
			{
				if (sf::Joystick::isButtonPressed(component->mControllerId, index))
				{
					component->mButtonId = index;
					break;
				}
			}
		}
#endif
	}
}

void InputSystem::CreatePlayerStates(std::vector<PlayerState*>& outPlayerStates)
{
	sf::Vector2i baseHudPos(120, 980);
	sf::Vector2i hudSpacing(540, 0);
#if _WIN64
	DWORD dwResult;
	for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
	{
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		// Simply get the state of the controller from XInput.
		dwResult = XInputGetState(i, &state);
		if (dwResult == ERROR_SUCCESS)
#elif
	for (int i = 0; i < sf::Joystick::Count; ++i)
	{
		if (sf::Joystick::isConnected(i))
#endif
		{
			sf::Vector2i playerHudPostion = baseHudPos + ((int)i * hudSpacing);
			outPlayerStates.emplace_back(new PlayerState(playerHudPostion));
		}
	}
}

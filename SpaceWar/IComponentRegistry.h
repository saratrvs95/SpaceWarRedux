#pragma once
#include <vector>

/**
 * The basis of every system. This is just an abstract class that encapsulates the storing,
 * registering and unregistering of components so the systems don't ahve to repeat the logic.
 */
template <typename T>
class IComponentRegistry
{
public:
	IComponentRegistry() = default;
	~IComponentRegistry();
	virtual void RegisterComponent(T* component);
	virtual void UnregisterComponent(T* component);

protected:
	std::vector<T*> mRegisteredComponents;
};

template<typename T>
inline IComponentRegistry<T>::~IComponentRegistry()
{
	mRegisteredComponents.clear();
}

template<typename T>
inline void IComponentRegistry<T>::RegisterComponent(T* component)
{
	if (!component) return;
	mRegisteredComponents.push_back(component);
	component = mRegisteredComponents.back();
}

template<typename T>
inline void IComponentRegistry<T>::UnregisterComponent(T* component)
{
	if (!component) return;

	for (auto it = mRegisteredComponents.begin(); it != mRegisteredComponents.end(); ++it)
	{
		if (component == *it)
		{
			mRegisteredComponents.erase(it);
			break;
		}
	}
}

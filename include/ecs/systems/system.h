#pragma once

#include <event_bus.h>

namespace NocEngine {

class ISystem
{
public:
	ISystem() = delete;
	ISystem(EventBus& eventBus)
		:m_eventBus(eventBus)
	{};
	virtual ~ISystem() = default;

	ISystem(const ISystem& other) = delete;
	ISystem& operator=(const ISystem& other) = delete;
	ISystem(ISystem&& other) noexcept = delete;
	ISystem& operator=(ISystem&& other) noexcept = delete;

	virtual void Update() = 0;

protected:
	EventBus& m_eventBus;
};

}
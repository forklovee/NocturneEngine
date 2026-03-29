#include "input_system.h"
#include <iostream>

namespace NocEngine {

InputSystem::InputSystem(EventBus& eventBus)
	:ISystem(eventBus)
{

	std::function<void(const KeyEvent&)> keyEvent =
		[this](const KeyEvent& event) { onKeyEvent(event); };
	m_eventBus.Subscribe<KeyEvent>(keyEvent);
};

void InputSystem::Update()
{

}

void InputSystem::onKeyEvent(const KeyEvent& event)
{
	m_keymap[event.scancode] = event.action;
}

}

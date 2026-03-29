#pragma once
#include <system.h>
#include <events.h>

namespace NocEngine {

class InputSystem : public ISystem
{
public:
	InputSystem(EventBus& eventBus);

	virtual void Update() override;

	bool IsKeyPressed(int scancode) const {
		auto it = m_keymap.find(scancode);
		return it != m_keymap.end() ? m_keymap.at(scancode) : false;
	}

	bool IsKeyReleased(int scancode) const {
		auto it = m_keymap.find(scancode);
		return it != m_keymap.end() ? !m_keymap.at(scancode) : false;
	}

private:
	void onKeyEvent(const KeyEvent& event);

private:
	std::unordered_map<int, uint8_t> m_keymap;
};

}
#pragma once

#include <event_bus.h>
#include <glm/ext/vector_float2.hpp>

namespace NocEngine {

	struct KeyEvent: public IEvent {
		int key, scancode, action, mods;

		KeyEvent(int key, int scancode, int action, int mods)
			:key(key), scancode(scancode), action(action), mods(mods)
		{
		}
	};

	struct WindowSizeChangedEvent: public IEvent {
		int width, height;

		WindowSizeChangedEvent(int width, int height)
			:width(width), height(height)
		{ }

		WindowSizeChangedEvent(glm::vec2 size)
			:width(size.x), height(size.y)
		{ }
	};

}
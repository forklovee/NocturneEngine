#pragma once

#include <vector>

namespace NocEngine {

    enum class EventType {
        DEFAULT,
        ENTITY_CREATED,
        ENTITY_DESTROYED
    };

    class IEventObserver {
        virtual ~IEventObserver() = default;
        virtual void OnEventTriggered(const class Entity& entity, class Event& event) = 0;
    };

    class Event {
    public:
        virtual ~Event();

        void Broadcast(...);
        void Subscribe(void (*function_callback)());
        void Unsubscribe(void (*function_callback)());

    private:
        EventType type{};
    };

} // NocEngine

//
// Created by forklovee on 18/01/2026.
//

#include "events/event_manager.h"

namespace NocEngine {

    Event::~Event() {

    }

    void Event::Broadcast(...) {
        //for (const IEventObserver& subscriber: m_subscribers) {
        //    subscriber.OnEventTriggered();
        //}
    }

    void Event::Subscribe(void (*function_callback)()) {

    }

    void Event::Unsubscribe(void (*function_callback)()) {

    }

} // NocEngine
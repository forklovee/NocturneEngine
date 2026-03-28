#pragma once

#include <cstdint>
#include <concepts>
#include "resource.h"

namespace NocEngine {
    template<typename T>
    concept ResourceType = std::is_base_of<IResource, T>::value;

    class ResourceManager;

    template<ResourceType T>
        struct Handle
    {
        Handle()
			: id(0), generation(0)
        { };
        Handle(uint32_t id)
            : id(id), generation(0)
        { }
        Handle(uint32_t id, uint32_t generation)
            : id(id), generation(generation)
        { }

        Handle(const Handle& other) {
            id = other.id;
            generation = other.generation;
        }
        Handle& operator=(const Handle& other)
        {
            id = other.id;
            generation = other.generation;
            return *this;
        }

        Handle(Handle&& other)
        {
            id = other.id;
            generation = other.generation;
            other.Invalidate();
        }
        Handle& operator=(Handle&& other) {
            id = other.id;
            generation = other.generation;
            other.Invalidate();
            return *this;
        }

        bool IsValid() const {
            return id != UINT32_MAX;
        }

        T* Get() const {
            return IsValid() ? ResourceManager::Get().Get(*this) : nullptr;
        }

        void Invalidate() {
            id = UINT32_MAX;
            generation = UINT32_MAX;
        }

        uint32_t id{UINT32_MAX};
        uint32_t generation{UINT32_MAX};
    };
}
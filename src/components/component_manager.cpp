//
// Created by forklovee on 17/01/2026.
//

#include "component_manager.h"

#include <assert.h>
#include <typeinfo>

namespace NocEngine {

    template<ValidComponent T>
    T& ComponentManager::GetComponent(const Entity& entity) const
    {
        const std::type_index index{typeid(T)};
        ComponentArray<T>& componentArray{m_components.at(index)};
        return componentArray.GetComponent(entity);
    }

    template<ValidComponent T>
    size_t ComponentManager::GetComponentBit()
    {
        return T::bit;
    }

    template <ValidComponent T>
    T& ComponentManager::CreateComponent(Entity& entity)
    {
        const std::type_index index{typeid(T)};
        if (!m_components.contains(index)) {
            m_components.insert({index, ComponentArray<T>()});
        }
        ComponentArray<T>& componentArray{m_components.at(index)};
        return componentArray.InsertComponent(entity, T{});
    }

    template<ValidComponent T>
    void ComponentManager::DestroyComponent(Entity &entity)
    {
        const std::type_index index{typeid(T)};
        assert(m_components.contains(index));
        ComponentArray<T>& componentArray{m_components.at(index)};
        componentArray.RemoveComponent(entity);
    }
} // NocEngine
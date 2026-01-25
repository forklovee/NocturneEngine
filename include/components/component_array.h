#pragma once

#include <concepts>
#include <cassert>
#include <unordered_map>
#include <vector>

#include "entities/entity.h"
#include "components.h"

namespace NocEngine {
    template<typename T>
    concept ValidComponent = std::is_base_of_v<CComponent, T>;

    // Necessary interface to store component arrays in ComponentManager.
    class IComponentArray {
    public:
        virtual ~IComponentArray() = default;
        virtual void OnEntityDestroyed(const Entity& entity) = 0;
    };

    template<ValidComponent T>
    class ComponentArray : public IComponentArray {
        typedef size_t EntityId;
        typedef size_t ComponentId;
        public:
            T& GetComponent(const Entity& entity) {
                const size_t entityId = entity.GetId();
                assert(m_entityToComponentMap.contains(entityId));

                const size_t componentId = m_entityToComponentMap.at(entityId);
                return m_components.at(componentId);
            }

            T& InsertComponent(const Entity& entity, T&& component) {
                const size_t entityId = entity.GetId();
                assert(!m_entityToComponentMap.contains(entityId));

                const size_t componentId = m_componentCount++;
                m_components.emplace_back(std::forward<T>(component));
                m_entityToComponentMap[entityId] = componentId;
                m_componentToEntityMap[componentId] = entityId;
                return m_components.at(componentId);
            }

            void RemoveComponent(const Entity& entity) {
                const size_t entityId = entity.GetId();
                auto it = m_entityToComponentMap.find(entityId);
                if (it == m_entityToComponentMap.end()) { // Entity not found - nothing to do.
                    return;
                }
                const size_t componentId = it->second;
                const size_t lastComponentId = m_componentCount - 1;
                if (componentId != lastComponentId){
                    const size_t lastComponentEntityId = m_componentToEntityMap.at(lastComponentId);
                    // Swap removed component with last component
                    m_components[componentId] = std::move(m_components[lastComponentId]);
                    m_entityToComponentMap[lastComponentEntityId] = componentId;
                    m_entityToComponentMap[componentId] = lastComponentEntityId;
                }
                m_components.pop_back();
                // Remove this entity and last component id from the map.
                m_entityToComponentMap.erase(entityId);
                m_componentToEntityMap.erase(lastComponentId);
                --m_componentCount;
            }

            void OnEntityDestroyed(const Entity& entity) override {
                const size_t entityId = entity.GetId();
                if (!m_entityToComponentMap.contains(entityId)) return;
                RemoveComponent(entity);
            }

        public:
            ComponentArray() {
                m_components.reserve(64);
                m_entityToComponentMap.reserve(64);
                m_componentToEntityMap.reserve(64);
            }

        private:
            std::vector<T> m_components{};
            std::unordered_map<EntityId, ComponentId> m_entityToComponentMap{};
            std::unordered_map<ComponentId, EntityId> m_componentToEntityMap{};
            size_t m_componentCount{};
    };

}
#pragma once
#include <array>
#include <vector>
#include <cassert>
#include <typeindex>
#include <unordered_map>

#include "components.h"
#include "entity.h"

namespace NocEngine {
    template<typename T>
    concept ValidComponent = std::is_base_of_v<CComponent, T> && requires
    {
        { T::bit } -> std::convertible_to<size_t>;
    };

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
                m_components[componentId] = component;
                m_entityToComponentMap.insert({ entityId, componentId });
                m_componentToEntityMap.insert({ componentId, entityId });
                return m_components.at(componentId);
            }

            void RemoveComponent(const Entity& entity) {
                const size_t entityId = entity.GetId();
                assert(m_entityToComponentMap.contains(entityId));

                const size_t componentId = m_entityToComponentMap.at(entityId);
                const size_t lastComponentId = --m_componentCount; //shrink size by 1
                if (m_componentCount == 0) {
                    // invalidated, don't have to swap anything.
                    return;
                }

                // Swap removed component with last component
                const size_t lastComponentEntityId = m_componentToEntityMap.at(lastComponentId);
                m_components[componentId] = std::move(m_components[lastComponentId]);
                // Remove this entity and last component id from the map.
                m_entityToComponentMap.erase(entityId);
                m_componentToEntityMap.erase(lastComponentId);
                // Now, swap last component's index it's entity.
                m_entityToComponentMap.at(lastComponentEntityId) = componentId;
                m_componentToEntityMap.insert({ componentId, lastComponentEntityId });
            }

            void OnEntityDestroyed(const Entity& entity) override {
                const size_t entityId = entity.GetId();
                assert(m_entityToComponentMap.contains(entityId));
                RemoveComponent(entity);
            }

        private:
            ComponentArray() {m_components.reserve(32);}

        private:
            std::vector<T> m_components{};
            std::unordered_map<EntityId, ComponentId> m_entityToComponentMap{};
            std::unordered_map<ComponentId, EntityId> m_componentToEntityMap{};
            size_t m_componentCount{};
    };


    class ComponentManager
    {
    public:
        static ComponentManager& Get() {
            static ComponentManager instance;
            return instance;
        }

        template<ValidComponent T>
        T& GetComponent(const Entity& entity) const;

        template<ValidComponent T>
        size_t GetComponentBit();

        template<ValidComponent T>
        T& CreateComponent(Entity& entity);

        template<ValidComponent T>
        void DestroyComponent(Entity& entity);

    private:
        ComponentManager() {};

    private:
        std::unordered_map<std::type_index, std::unique_ptr<IComponentArray>> m_components{};

    };
} // NocEngine

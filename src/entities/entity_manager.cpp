#include "entities/entity_manager.h"
#include "component_manager.h"
#include <memory>
#include <ranges>

namespace NocEngine {

void EntityManager::Update() {
    auto& cm = ComponentManager::Get();

    // Add new entities
    if (!m_entitiesToAdd.empty()) {
        for (Entity& entity: m_entitiesToAdd) {
            entity.m_id = m_entities.size();
            m_entities.emplace_back(std::move(entity));
        }
        m_entitiesToAdd.clear();
    }

    // Destroy dead entities
    // and Notify ComponentManager about entity deaths to destroy their components
    size_t idx{0};
    while(idx < m_entities.size()){
        Entity& entity{m_entities[idx]};
        if (entity.IsAlive()){
            idx++;
            continue;
        }
        cm.OnEntityDestroyed(entity);

        // Swap-pop
        size_t lastIndex = m_entities.size()-1;
        if (lastIndex != idx){
            m_entities[idx] = std::move(m_entities[lastIndex]);
        }
        m_entities.pop_back();
    }
}

std::vector<Entity*> EntityManager::GetAllEntities()
{
    std::vector<Entity*> entity_ptrs{};
    entity_ptrs.reserve(m_entities.size());
    for (Entity& e: m_entities){
        entity_ptrs.push_back(&e);
    }
    return entity_ptrs;
}


Entity& EntityManager::CreateEntity() {
    size_t entityId{m_entities.size()};
    m_entitiesToAdd.emplace_back(Entity());
    return m_entitiesToAdd.back();
}

bool EntityManager::isEntityMarkedForDeletion(
    const std::shared_ptr<Entity> &entity) {
  return !entity || !entity->IsAlive();
}

}
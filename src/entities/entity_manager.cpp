#include "../../include/entities/entity_manager.h"
#include <memory>
#include <ranges>

namespace NocEngine {

void EntityManager::Update() {
    // Add new entities
    if (!m_entitiesToAdd.empty()) {
        for (Entity& entity: m_entitiesToAdd) {
            m_entities.push_back(std::move(entity));
        }
        m_entitiesToAdd.clear();
    }

    // Destroy dead entities
    std::erase_if(m_entities, [](const Entity& e) {return !e.IsAlive();});
}

std::vector<Entity>& EntityManager::GetEntities() {
    return m_entities;
}

Entity& EntityManager::CreateEntity() {
    m_entitiesToAdd.push_back(
        Entity(m_totalEntities++)
        );
    return m_entitiesToAdd.back();
}

bool EntityManager::isEntityMarkedForDeletion(
    const std::shared_ptr<Entity> &entity) {
  return !entity || !entity->IsAlive();
}

} // namespace NocEngine

#include "../../include/entities/entity_manager.h"
#include <memory>

namespace NocEngine {

void EntityManager::Update() {
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

void EntityManager::RemoveEntity(const Entity& entity) {
    m_entitiesToRemove.push_back(entity.GetId());
}

bool EntityManager::isEntityMarkedForDeletion(
    const std::shared_ptr<Entity> &entity) {
  return !entity || !entity->IsAlive();
}

} // namespace NocEngine

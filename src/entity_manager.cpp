#include "entity_manager.h"
#include <memory>

namespace NocEngine {

EntityManager::EntityManager() {}

void EntityManager::Update() {
  // add new entities
  for (auto &e : m_entitiesToAdd) {
    m_entities.push_back(e);
    m_entityMap[e->GetType()].push_back(e);
  }
  m_entitiesToAdd.clear();

  // remove dead entities
  std::erase_if(m_entities, EntityManager::isEntityMarkedForDeletion);
  for (auto &pair : m_entityMap) {
    std::erase_if(pair.second, EntityManager::isEntityMarkedForDeletion);
  }
}

std::shared_ptr<Entity> EntityManager::AddEntity(const EntityType entityType) {
  auto e = std::shared_ptr<Entity>(new Entity(entityType, m_totalEntities++));
  m_entitiesToAdd.push_back(e);
  return e;
}

EntityVec &EntityManager::GetEntities() { return m_entities; }

EntityVec &EntityManager::GetEntities(const EntityType entityType) {
  return m_entityMap.at(entityType);
}

bool EntityManager::isEntityMarkedForDeletion(
    const std::shared_ptr<Entity> &entity) {
  return !entity || !entity->IsAlive();
}

} // namespace NocEngine

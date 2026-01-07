#pragma once

#include <map>
#include <memory>
#include <vector>

#include "entity.h"

namespace NocEngine {

typedef std::vector<std::shared_ptr<Entity>> EntityVec;
typedef std::map<EntityType, EntityVec> EntityMap;

class EntityManager {
public:
  EntityManager();

  void Update();

  std::shared_ptr<Entity> AddEntity(const EntityType entityType);
  EntityVec &GetEntities();
  EntityVec &GetEntities(const EntityType entityType);

private:
  static bool isEntityMarkedForDeletion(const std::shared_ptr<Entity> &entity);

private:
  EntityVec m_entities;
  EntityVec m_entitiesToAdd;
  EntityMap m_entityMap;
  size_t m_totalEntities{};
};

} // namespace NocEngine

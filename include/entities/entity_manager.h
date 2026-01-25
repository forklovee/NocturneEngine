#pragma once

#include <map>
#include <memory>
#include <vector>

#include "entity.h"

namespace NocEngine {

class EntityManager {
  public:
    static EntityManager& Get() {
      static EntityManager entity_manager;
      return entity_manager;
    }

    void Update();

    std::vector<Entity*> GetAllEntities();

    Entity& CreateEntity();

  private:
    EntityManager() {
      m_entities.reserve(128);
      m_entitiesToAdd.reserve(128);
    }

    static bool isEntityMarkedForDeletion(const std::shared_ptr<Entity> &entity);

  private:
    std::vector<Entity> m_entities{};
    std::vector<Entity> m_entitiesToAdd{};
  };

} // namespace NocEngine

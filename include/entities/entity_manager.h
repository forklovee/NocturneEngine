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

    std::vector<Entity>& GetEntities();
    Entity& CreateEntity();
    void RemoveEntity(const Entity& entity);


  private:
    EntityManager() {};

    static bool isEntityMarkedForDeletion(const std::shared_ptr<Entity> &entity);

  private:
    std::vector<Entity> m_entities{};
    std::vector<Entity> m_entitiesToAdd{};
    std::vector<size_t> m_entitiesToRemove{};
    size_t m_totalEntities{};
  };

} // namespace NocEngine

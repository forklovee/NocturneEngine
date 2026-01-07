#pragma once

#include <cstddef>
#include <memory>

namespace NocEngine {

enum class EntityType { DEFAULT, CHARACTER, PROJECTILE };
class EntityManager;

class Entity {
  friend class EntityManager;

public:
  void Destroy() { m_isAlive = false; }

  const EntityType GetType() const { return m_type; };
  const bool IsAlive() const { return m_isAlive; };

private:
  Entity();
  Entity(EntityType type, size_t id);

private:
  const size_t m_id{};
  const EntityType m_type{EntityType::DEFAULT};
  bool m_isAlive{true};
};

} // namespace NocEngine

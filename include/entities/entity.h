#pragma once

#include <bitset>
#include <memory>
#include <typeindex>
#include <unordered_map>

namespace NocEngine {

class EntityManager;

  class Entity {
    friend class EntityManager;
    friend class ComponentManager;

  public:
    size_t GetId() const { return m_id; }
    void Destroy() { m_isAlive = false; }
    bool IsAlive() const { return m_isAlive; };

    const std::bitset<64>& GetComponentBits() const;

  private:
    Entity();
    Entity(const size_t id);

  private:
    std::bitset<64> m_componentBits;
    std::unordered_map<std::type_index, size_t> m_componentIndex{};

    size_t m_id{};
    bool m_isAlive{true};
  };

} // namespace NocEngine

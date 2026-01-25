#include "../../include/entities/entity.h"

#include <iostream>

namespace NocEngine {
    const std::bitset<64> & Entity::GetComponentBits() const {
        return m_componentBits;
    }
} // namespace NocEngine

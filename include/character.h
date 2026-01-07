#pragma once

#include "components.h"
#include "entity.h"

namespace NocEngine {
class Character : public Entity {
private:
  std::shared_ptr<CTransform2D> cTransform;
  std::shared_ptr<CBoxShape2D> cAABB;
};
} // namespace NocEngine

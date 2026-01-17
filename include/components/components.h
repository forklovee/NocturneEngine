#pragma once

#include "../math.h"

namespace NocEngine {



struct CComponent {
    enum ComponentType {
        TRANSFORM = 0,
        BOX_SHAPE = 1,
        SPHERE_SHAPE = 2,
        MESH_RENDERER = 3,
    };
};

struct CTransform: CComponent {
    static constexpr ComponentType bit = TRANSFORM;

  Math::Vector3<float> position{};
  Math::Vector3<float> rotation{};
  Math::Vector3<float> scale{};

  CTransform(const Math::Vector3<float> &position,
               const Math::Vector3<float> &rotation, const Math::Vector3<float> &scale)
      : position(position), rotation(rotation), scale(scale) {}
};

struct CBoxShape: CComponent {
    static constexpr ComponentType bit = BOX_SHAPE;

  Math::Vector3<float> position{};
  Math::Vector3<float> size{};

  CBoxShape(const Math::Vector3<float> &position,
              const Math::Vector3<float> &size)
      : position(position), size(size) {}
};

struct CSphereShape: CComponent {
    static constexpr ComponentType bit = SPHERE_SHAPE;

    float radius{};

  CSphereShape(float radius) : radius(radius) {}
};

} // namespace NocEngine

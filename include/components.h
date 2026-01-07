#pragma once

#include "math.h"

namespace NocEngine {

struct CTransform2D {
  Math::Vector2<float> position{};
  Math::Vector2<float> velocity{};
  float angle{};

  CTransform2D(const Math::Vector2<float> &position,
               const Math::Vector2<float> &velocity, float angle)
      : position(position), velocity(velocity), angle(angle) {}
};

struct CBoxShape2D {
  Math::Vector2<float> position{};
  Math::Vector2<float> size{};

  CBoxShape2D(const Math::Vector2<float> &position,
              const Math::Vector2<float> size)
      : position(position), size(size) {}
};

struct CCircleShape2D {
  float radius{};

  CCircleShape2D(float radius) : radius(radius) {}
};

struct CLifespan {
  int remaining{};
  int total{};

  CLifespan(int total) : total(total), remaining(total) {}
};

struct CInput {
  bool up{}, left{}, right{}, down{};

  CInput() {}
};

} // namespace NocEngine

#pragma once

#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <cstdint>

#include "math.h"

namespace NocEngine {

struct CComponent {
};

struct CTransform: CComponent {
  Math::Vector3<float> position{};
  Math::Vector3<float> rotation{};
  Math::Vector3<float> scale{};
};

struct CMeshRenderer: CComponent{
  struct MeshData* mesh_ptr;
  class Shader* shader_ptr;

  uint32_t VAO{};
  uint32_t VBO{};
  uint32_t EBO{};

  // :(
  // No separation of systems and data
  // I know... I feel bad about this, but will fix it later!!!
  ~CMeshRenderer() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
  }
};

struct CBoxShape: CComponent {
  Math::Vector3<float> position{};
  Math::Vector3<float> size{};
};

struct CSphereShape: CComponent {
  float radius{};
};

}

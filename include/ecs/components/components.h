#pragma once

#include <glm/vec3.hpp>
#include <cstdint>

#include "handle.h"
#include "texture.h"
#include "meshdata.h"
#include "material.h"
#include "gpu_mesh.h"
#include "gpu_texture.h"

class MeshData;
class Shader;

namespace NocEngine {

struct CComponent {
};

struct CTransform: CComponent {
  glm::vec3 position{};
  // Rotation in radians, applied in XYZ order
  glm::vec3 rotation{};
  glm::vec3 scale{1.f};
};

struct CMeshRenderer: CComponent{
	Handle<MeshData> mesh{};
	Handle<Material> material{};

	GPU_Mesh* gpu_mesh{};
	GPU_Texture* gpu_texture{};
};

struct CBoxShape: CComponent {
  glm::vec3 position{};
  glm::vec3 size{};
};

struct CSphereShape: CComponent {
  float radius{};
};

}

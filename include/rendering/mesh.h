#pragma once

#include <cstdint>
#include <vector>

namespace NocEngine
{

// Struct representing loaded mesh data from external 3d model
struct MeshData
{
    MeshData() {
        // Temporary, just for testing purposes
        // Plane Mesh
        vertices = {
          0.5f,  0.5f, 0.0f,  // top right
          0.5f, -0.5f, 0.0f,  // bottom right
          -0.5f, -0.5f, 0.0f,  // bottom left
          -0.5f,  0.5f, 0.0f   // top left 
        };

        indices = {
        0, 1, 3,
        1, 2, 3
        };
        use_indices = true;
    }

    // MeshData(const char* file_path);

    std::vector<float> vertices{};
    std::vector<uint32_t> indices{};
    bool use_indices{};
};

}
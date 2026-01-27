#pragma once

#include <bitset>
#include "components.h"
#include "entity.h"

namespace NocEngine {

class RenderingSystem
{
    public:
        void Update();

    private:
        const std::bitset<64> getRequiredBitmask() const;
        void renderEntity(Entity entity);

        bool isMeshLoadedOnGPU(const CMeshRenderer& mr) const;
        void loadToGPU(CMeshRenderer& mr) const;
};

}
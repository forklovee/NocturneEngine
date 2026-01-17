#pragma once
#include <memory>

#include "entity.h"

namespace NocEngine
{
class Actor3D: public Entity
{
public:
    Actor3D();

private:
    std::shared_ptr<class CTransform3D> transform;
};

}

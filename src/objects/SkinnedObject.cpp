#include "SkinnedObject.h"


namespace renderbox {

    SkinnedObject::SkinnedObject(
            std::shared_ptr<Geometry> const &geometry,
            std::shared_ptr<Material> const &material)
            : Object(geometry, material) {}

}

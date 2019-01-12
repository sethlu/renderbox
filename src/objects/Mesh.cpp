#include "Mesh.h"


namespace renderbox {

    Mesh::Mesh(std::shared_ptr<Geometry> const &geometry, std::shared_ptr<Material> const &material)
        : Object(geometry, material) {

    }

}

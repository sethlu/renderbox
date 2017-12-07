#include "Mesh.h"


namespace renderbox {

    Mesh::Mesh(std::shared_ptr<Geometry> geometry, std::shared_ptr<Material> material)
        : Object(geometry, material) {

    }

}

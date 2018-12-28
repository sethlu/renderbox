#include "SkinnedMesh.h"

#include <utility>


namespace renderbox {

    SkinnedMesh::SkinnedMesh(std::shared_ptr<Geometry> geometry, std::shared_ptr<Material> material)
            : Mesh(std::move(geometry), std::move(material)) {

    }

}

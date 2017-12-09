#include "MeshBasicMaterial.h"


namespace renderbox {

    MeshBasicMaterial::MeshBasicMaterial(glm::vec3 color)
        : ColorMaterial(color) {

    }

    MATERIAL MeshBasicMaterial::getMaterialType() const {
        return MESH_BASIC_MATERIAL;
    };

}

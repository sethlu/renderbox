#include "MeshBasicMaterial.h"


namespace renderbox {

    MeshBasicMaterial::MeshBasicMaterial(glm::vec3 color) {
        this->color = color;
    }

    MATERIAL MeshBasicMaterial::getMaterialType() {
        return MESH_BASIC_MATERIAL;
    }

}
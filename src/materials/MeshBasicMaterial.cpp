#include "MeshBasicMaterial.h"


namespace renderbox {

    MeshBasicMaterial::MeshBasicMaterial(glm::vec3 color) {
        this->color = color;
    }

    glm::vec3 MeshBasicMaterial::getColor() const {
        return color;
    }

    MATERIAL MeshBasicMaterial::getMaterialType() const {
        return MESH_BASIC_MATERIAL;
    };

}

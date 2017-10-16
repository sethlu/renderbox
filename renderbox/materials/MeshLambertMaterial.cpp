#include "MeshLambertMaterial.h"


namespace renderbox {

    MeshLambertMaterial::MeshLambertMaterial(glm::vec3 color) {
        this->color = color;
    }

    MATERIAL MeshLambertMaterial::getMaterialType() {
        return MESH_LAMBERT_MATERIAL;
    }

}
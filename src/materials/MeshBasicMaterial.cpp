#include "MeshBasicMaterial.h"


namespace renderbox {

    MeshBasicMaterial::MeshBasicMaterial(glm::vec3 color)
        : DiffuseMaterial(color) {

    }

    MATERIAL_TYPE MeshBasicMaterial::getMaterialType() const {
        return MESH_BASIC_MATERIAL;
    };

}

#include "MeshLambertMaterial.h"


namespace renderbox {

    MeshLambertMaterial::MeshLambertMaterial(glm::vec3 color)
        : ColorMaterial(color) {

    }

    MATERIAL_TYPE MeshLambertMaterial::getMaterialType() const {
        return MESH_LAMBERT_MATERIAL;
    };

}

#include "MeshLambertMaterial.h"


namespace renderbox {

    MeshLambertMaterial::MeshLambertMaterial(glm::vec3 color)
        : color(color) {

    }

    glm::vec3 MeshLambertMaterial::getColor() const {
        return color;
    };

    MATERIAL MeshLambertMaterial::getMaterialType() const {
        return MESH_LAMBERT_MATERIAL;
    };

}

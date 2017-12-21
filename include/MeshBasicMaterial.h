#ifndef VOX_MESHBASICMATERIAL_H
#define VOX_MESHBASICMATERIAL_H


#include "Material.h"

namespace renderbox {

    class MeshBasicMaterial : public Material, public DiffuseMaterial {
    public:

        explicit MeshBasicMaterial(glm::vec3 diffuseColor = glm::vec3(1.0f))
            : DiffuseMaterial(diffuseColor) {

        };

        MATERIAL_TYPE getMaterialType() const override {
            return MESH_BASIC_MATERIAL;
        };

    };

}


#endif //VOX_MESHBASICMATERIAL_H

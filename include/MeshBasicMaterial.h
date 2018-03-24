#ifndef VOX_MESHBASICMATERIAL_H
#define VOX_MESHBASICMATERIAL_H


#include "Material.h"

namespace renderbox {

    class MeshBasicMaterial : public Material, public DiffuseMaterial {
    public:

        explicit MeshBasicMaterial(vec3 diffuseColor = vec3(1.0f))
            : DiffuseMaterial(diffuseColor) {

        };

        MATERIAL_TYPE getMaterialType() const override {
            return MESH_BASIC_MATERIAL;
        };

    };

}


#endif //VOX_MESHBASICMATERIAL_H

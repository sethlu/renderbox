#ifndef VOX_MESHLAMBERTMATERIAL_H
#define VOX_MESHLAMBERTMATERIAL_H


#include "Material.h"

namespace renderbox {

    class MeshLambertMaterial : public Material, public AmbientMaterial, public DiffuseMaterial {
    public:

        explicit MeshLambertMaterial(glm::vec3 diffuseColor = glm::vec3(1.0f))
            : MeshLambertMaterial(glm::vec3(1.0f), diffuseColor) {

        };

        MeshLambertMaterial(glm::vec3 ambientColor,
                            glm::vec3 diffuseColor)
            : AmbientMaterial(ambientColor), DiffuseMaterial(diffuseColor) {

        };

        MATERIAL_TYPE getMaterialType() const override {
            return MESH_LAMBERT_MATERIAL;
        };

    };

}


#endif //VOX_MESHLAMBERTMATERIAL_H

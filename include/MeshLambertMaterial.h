#ifndef RENDERBOX_MESHLAMBERTMATERIAL_H
#define RENDERBOX_MESHLAMBERTMATERIAL_H


#include "Material.h"

namespace renderbox {

    class MeshLambertMaterial : public Material, public AmbientMaterial, public DiffuseMaterial {
    public:

        explicit MeshLambertMaterial(vec3 diffuseColor = vec3(1.0f))
                : MeshLambertMaterial(vec3(1.0f), diffuseColor) {

        };

        MeshLambertMaterial(vec3 ambientColor,
                            vec3 diffuseColor)
                : AmbientMaterial(ambientColor), DiffuseMaterial(diffuseColor) {

        };

        MATERIAL_TYPE getMaterialType() const override {
            return MESH_LAMBERT_MATERIAL;
        };

    };

}


#endif //RENDERBOX_MESHLAMBERTMATERIAL_H

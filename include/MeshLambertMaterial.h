#ifndef VOX_MESHLAMBERTMATERIAL_H
#define VOX_MESHLAMBERTMATERIAL_H


#include "Material.h"

namespace renderbox {

    class MeshLambertMaterial : public Material, public DiffuseMaterial {
    public:

        explicit MeshLambertMaterial(glm::vec3 color = glm::vec3(1.0f));

        MATERIAL_TYPE getMaterialType() const override;

    };

}


#endif //VOX_MESHLAMBERTMATERIAL_H

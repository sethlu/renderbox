#ifndef VOX_MESHBASICMATERIAL_H
#define VOX_MESHBASICMATERIAL_H


#include "Material.h"

namespace renderbox {

    class MeshBasicMaterial : public Material, public DiffuseMaterial {
    public:

        explicit MeshBasicMaterial(glm::vec3 color);

        MATERIAL_TYPE getMaterialType() const override;

    };

}


#endif //VOX_MESHBASICMATERIAL_H

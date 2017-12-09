#ifndef VOX_MESHBASICMATERIAL_H
#define VOX_MESHBASICMATERIAL_H


#include "Material.h"

namespace renderbox {

    class MeshBasicMaterial : public Material, public ColorMaterial {
    public:

        explicit MeshBasicMaterial(glm::vec3 color);

        MATERIAL getMaterialType() const override;

    };

}


#endif //VOX_MESHBASICMATERIAL_H

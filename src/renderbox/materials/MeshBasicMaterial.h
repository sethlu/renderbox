#ifndef VOX_MESHBASICMATERIAL_H
#define VOX_MESHBASICMATERIAL_H


#include <glm/vec3.hpp>
#include "Material.h"

namespace renderbox {

    class MeshBasicMaterial : public Material {
    public:
        glm::vec3 color;
        MeshBasicMaterial(glm::vec3 color);
        MATERIAL getMaterialType();
    };

}


#endif //VOX_MESHBASICMATERIAL_H

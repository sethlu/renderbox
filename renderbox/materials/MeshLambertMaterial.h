#ifndef VOX_MESHLAMBERTMATERIAL_H
#define VOX_MESHLAMBERTMATERIAL_H


#include <glm/vec3.hpp>
#include "Material.h"

namespace renderbox {

    class MeshLambertMaterial : public Material {
    public:
        glm::vec3 color;
        MeshLambertMaterial(glm::vec3 color);
        MATERIAL getMaterialType();
    };

}


#endif //VOX_MESHLAMBERTMATERIAL_H

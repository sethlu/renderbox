#ifndef PARTICLESIM_MESHLAMBERTMATERIAL_H
#define PARTICLESIM_MESHLAMBERTMATERIAL_H


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


#endif //PARTICLESIM_MESHLAMBERTMATERIAL_H

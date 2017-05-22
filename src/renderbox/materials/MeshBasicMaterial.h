#ifndef PARTICLESIM_MESHBASICMATERIAL_H
#define PARTICLESIM_MESHBASICMATERIAL_H


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


#endif //PARTICLESIM_MESHBASICMATERIAL_H

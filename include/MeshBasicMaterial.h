#ifndef VOX_MESHBASICMATERIAL_H
#define VOX_MESHBASICMATERIAL_H


#include <glm/vec3.hpp>
#include "Material.h"

namespace renderbox {

    class MeshBasicMaterial : public Material {
    protected:

        glm::vec3 color;

    public:

        MeshBasicMaterial(glm::vec3 color);

        glm::vec3 getColor() const;

        MATERIAL getMaterialType() const override;

    };

}


#endif //VOX_MESHBASICMATERIAL_H

#ifndef VOX_MESHLAMBERTMATERIAL_H
#define VOX_MESHLAMBERTMATERIAL_H


#include <glm/vec3.hpp>
#include "Material.h"

namespace renderbox {

    class MeshLambertMaterial : public Material {
    protected:

        glm::vec3 color;

    public:

        MeshLambertMaterial(glm::vec3 color);

        glm::vec3 getColor() const;

        MATERIAL getMaterialType() const override;

    };

}


#endif //VOX_MESHLAMBERTMATERIAL_H

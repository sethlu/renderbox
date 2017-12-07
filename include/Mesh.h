#ifndef VOX_MESH_H
#define VOX_MESH_H


#include "Geometry.h"
#include "Material.h"
#include "Object.h"

namespace renderbox {

    class Mesh : public Object {
    public:

        Mesh(std::shared_ptr<Geometry> geometry, std::shared_ptr<Material> material);

    };

}


#endif //VOX_MESH_H

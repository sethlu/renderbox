#ifndef VOX_MESH_H
#define VOX_MESH_H


#include "Geometry.h"
#include "Material.h"
#include "Object.h"

namespace renderbox {

    class Mesh : public Object {
    public:

        Mesh(Geometry *geometry, Material *material);

    };

}


#endif //VOX_MESH_H

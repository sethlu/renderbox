#ifndef VOX_MESH_H
#define VOX_MESH_H


#include "../geometries/Geometry.h"
#include "../materials/Material.h"
#include "../core/Object.h"

namespace renderbox {

    class Mesh : public Object {
    public:
        Mesh(Geometry *geometry, Material *material);
    };

}


#endif //VOX_MESH_H

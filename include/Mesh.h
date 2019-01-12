#ifndef RENDERBOX_MESH_H
#define RENDERBOX_MESH_H


#include "Geometry.h"
#include "Material.h"
#include "Object.h"


namespace renderbox {

    class Mesh : public Object {
    public:

        Mesh(std::shared_ptr<Geometry> const &geometry = nullptr, std::shared_ptr<Material> const &material = nullptr);

        OBJECT_TYPE getObjectType() const override {
            return MESH_OBJECT;
        };

    };

}


#endif //RENDERBOX_MESH_H

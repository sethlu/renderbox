#ifndef RENDERBOX_MESH_H
#define RENDERBOX_MESH_H


#include "Bone.h"


namespace renderbox {

    class SkinnedObject : public Object {
    public:

        SkinnedObject(
                std::shared_ptr<Geometry> const &geometry = nullptr,
                std::shared_ptr<Material> const &material = nullptr);

        OBJECT_TYPE getObjectType() const override {
            return SKINNED_OBJECT;
        };

        std::vector<std::shared_ptr<Bone>> bones;

    };

}


#endif //RENDERBOX_MESH_H

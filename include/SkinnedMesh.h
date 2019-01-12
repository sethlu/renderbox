#ifndef RENDERBOX_SKINNEDMESH_H
#define RENDERBOX_SKINNEDMESH_H


#include "Mesh.h"
#include "Bone.h"


namespace renderbox {

    class SkinnedMesh : public Mesh {
    public:

        SkinnedMesh(std::shared_ptr<Geometry> geometry, std::shared_ptr<Material> material);

        std::vector<std::shared_ptr<Bone>> bones;

    };

}


#endif //RENDERBOX_SKINNEDMESH_H

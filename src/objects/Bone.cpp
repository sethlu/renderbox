#include "Bone.h"


namespace renderbox {

    void Bone::bindMatrix(bool recursive) {
        boneInverse = glm::inverse(getWorldMatrix());
        if (recursive) {
            for (auto const &child : children) {
                if (child->isBone()) {
                    if (auto bone = dynamic_cast<Bone *>(child.get())) {
                        bone->bindMatrix(recursive);
                    }
                }
            }
        }
    }

    mat4 Bone::getBoneInverse() const {
        return boneInverse;
    }

}

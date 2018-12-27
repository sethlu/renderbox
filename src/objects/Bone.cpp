#include "Bone.h"


namespace renderbox {

    void Bone::bindMatrix() {
        boneInverse = glm::inverse(getWorldMatrix());
    }

    mat4 Bone::getBoneInverse() const {
        return boneInverse;
    }

}

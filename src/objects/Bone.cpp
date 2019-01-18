#include "Bone.h"


namespace renderbox {

    void Bone::bindBoneMatrix(bool recursive) {
        boundBoneMatrixInverse = glm::inverse(boneMatrix);
        if (recursive) {
            for (auto const &child : children) {
                if (child->isBone()) {
                    if (auto bone = dynamic_cast<Bone *>(child.get())) {
                        bone->bindBoneMatrix(recursive);
                    }
                }
            }
        }
    }

    mat4 Bone::getBoneMatrix() const {
        return boneMatrix;
    }

    mat4 Bone::getBoundBoneMatrixInverse() const {
        return boundBoneMatrixInverse;
    }

    void Bone::didTransform() {
        boneMatrix = getMatrix();
        if (hasParent() && parent->isBone()) {
            if (auto parentBone = dynamic_cast<Bone *>(parent)) {
                boneMatrix = parentBone->boneMatrix * boneMatrix;
            }
        }
        Object::didTransform();
    }

}

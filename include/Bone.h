#ifndef RENDERBOX_BONE_H
#define RENDERBOX_BONE_H


#include "Object.h"


namespace renderbox {

    class Bone : public Object {
    public:

        OBJECT_TYPE getObjectType() const override {
            return BONE_OBJECT;
        }

        void bindBoneMatrix(bool recursive = false);

        mat4 getBoneMatrix() const;

        mat4 getBoundBoneMatrixInverse() const;

    protected:

        mat4 boneMatrix;

        mat4 boundBoneMatrixInverse;

        void didTransform() override;

    };

}


#endif //RENDERBOX_BONE_H

#ifndef RENDERBOX_BONE_H
#define RENDERBOX_BONE_H


#include "Object.h"


namespace renderbox {

    class Bone : public Object {
    public:

        OBJECT_TYPE getObjectType() const override {
            return BONE_OBJECT;
        }

        void bindMatrix(bool recursive = false);

        mat4 getBoneInverse() const;

    protected:

        mat4 boneInverse;

    };

}


#endif //RENDERBOX_BONE_H

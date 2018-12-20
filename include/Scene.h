#ifndef RENDERBOX_SCENE_H
#define RENDERBOX_SCENE_H


#include "Object.h"

namespace renderbox {

    class Scene : public Object {
    public:

        Scene();

        vec3 getAmbientColor() const;

        OBJECT_TYPE getObjectType() const override {
            return SCENE_OBJECT;
        };

        void setAmbientColor(const vec3 &ambientColor);

    protected:

        vec3 ambientColor;

    };

}


#endif //RENDERBOX_SCENE_H

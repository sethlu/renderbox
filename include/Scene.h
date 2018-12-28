#ifndef RENDERBOX_SCENE_H
#define RENDERBOX_SCENE_H


#include "Object.h"
#include "AnimationMixer.h"


namespace renderbox {

    class Scene : public Object {
    public:

        Scene();

        vec3 getAmbientColor() const;

        OBJECT_TYPE getObjectType() const override {
            return SCENE_OBJECT;
        };

        void setAmbientColor(const vec3 &ambientColor);

        std::vector<std::unique_ptr<AnimationMixer>> animationMixers;

        void updateAnimations(float t);

    protected:

        vec3 ambientColor;

    };

}


#endif //RENDERBOX_SCENE_H

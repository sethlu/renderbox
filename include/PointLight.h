#ifndef RENDERBOX_POINTLIGHT_H
#define RENDERBOX_POINTLIGHT_H


#include "Light.h"

namespace renderbox {

    class PointLight : public Light, public ColorLight {
    public:

        explicit PointLight(glm::vec3 color = glm::vec3(1), float distance = 0, float decay = 1)
                : ColorLight(color), distance(distance), decay(decay) {};

        LIGHT_TYPE getLightType() const override {
            return POINT_LIGHT;
        };

        float distance;

        float decay;

    };

}


#endif //RENDERBOX_POINTLIGHT_H

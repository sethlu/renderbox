#ifndef RENDERBOX_LIGHT_H
#define RENDERBOX_LIGHT_H


#include "Object.h"

namespace renderbox {

    enum LIGHT_TYPE {
        POINT_LIGHT = 0x01,
    };

    class Light : public Object {
    public:

        OBJECT_TYPE getObjectType() const override {
            return LIGHT_OBJECT;
        };

        virtual LIGHT_TYPE getLightType() const = 0;

    };

    class ColorLight {
    public:

        explicit ColorLight(vec3 color) : color(color) {};

        vec3 getColor() { return color; };

    protected:

        vec3 color;

    };

}


#endif //RENDERBOX_LIGHT_H

#ifndef VOX_RAY_H
#define VOX_RAY_H


#include "Object.h"

namespace renderbox {

    class Ray {

        vec3 origin;

        vec3 direction;

    public:

        Ray(vec3 origin, vec3 direction);

        vec3 getOrigin() const;

        vec3 getDirection() const;

        bool intersectTriangle(vec3 &v0, vec3 &v1, vec3 &v2, vec2 &baryPosition, float &distance);

        bool intersectObject(Object *object, std::vector<vec3> &positions);

        Ray *copy(mat4 changeOfCoordinates);

    };

}


#endif //VOX_RAY_H

#ifndef RENDERBOX_RAY_H
#define RENDERBOX_RAY_H


#include "Object.h"

namespace renderbox {

    class Ray {

        friend Ray operator*(const mat4 &matrix, const Ray &ray);

    public:

        Ray(vec3 origin, vec3 direction);

        vec3 getOrigin() const;

        vec3 getDirection() const;

        bool intersectTriangle(vec3 &v0, vec3 &v1, vec3 &v2, vec2 &baryPosition, float &distance);

        bool intersectObject(Object *object, std::vector<vec3> &positions);

    protected:

        vec3 origin;

        vec3 direction;

    };

    Ray operator*(const mat4 &matrix, const Ray &ray);

}


#endif //RENDERBOX_RAY_H

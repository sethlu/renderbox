#ifndef VOX_RAY_H
#define VOX_RAY_H


#include <glm/vec3.hpp>
#include "../core/Object.h"

namespace renderbox {

    class Ray {
        glm::vec3 origin;
        glm::vec3 direction;
    public:
        Ray(glm::vec3 origin, glm::vec3 direction);
        Ray *copy(glm::mat4x4 changeOfCoordinates);
        glm::vec3 getOrigin();
        glm::vec3 getDirection();
        bool intersectTriangle(glm::vec3 &v0, glm::vec3 &v1, glm::vec3 &v2, glm::vec3 &baryPosition);
        bool intersectObject(Object *object, std::vector<glm::vec3> &positions);
    };

}


#endif //VOX_RAY_H

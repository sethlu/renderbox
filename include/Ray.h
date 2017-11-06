#ifndef VOX_RAY_H
#define VOX_RAY_H


#include <glm/vec3.hpp>
#include "Object.h"

namespace renderbox {

    class Ray {

        glm::vec3 origin;

        glm::vec3 direction;

    public:

        Ray(glm::vec3 origin, glm::vec3 direction);

        glm::vec3 getOrigin() const;

        glm::vec3 getDirection() const;

        bool intersectTriangle(glm::vec3 &v0, glm::vec3 &v1, glm::vec3 &v2, glm::vec3 &baryPosition);

        bool intersectObject(Object *object, std::vector<glm::vec3> &positions);

        Ray *copy(glm::mat4x4 changeOfCoordinates);

    };

}


#endif //VOX_RAY_H

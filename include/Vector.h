#ifndef VOX_VECTOR_H
#define VOX_VECTOR_H


#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <cstdio>

namespace renderbox {

    void printVector(glm::vec3 vector);

    inline glm::vec3 dehomogenize(glm::vec4 vector) {
        return glm::vec3(vector) / vector.w;
    };

}

#endif //VOX_VECTOR_H

#ifndef PARTICLESIM_VECTOR_H
#define PARTICLESIM_VECTOR_H


#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <cstdio>

namespace renderbox {

    void printVector(glm::vec3 vector);

    glm::vec3 dehomogenize(glm::vec4 vector);

}

#endif //PARTICLESIM_VECTOR_H

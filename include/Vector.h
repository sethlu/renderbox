#ifndef VOX_VECTOR_H
#define VOX_VECTOR_H


#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtx/string_cast.hpp>
#include <cstdio>

namespace {

    std::ostream &operator<<(std::ostream &os, const glm::vec3 &vector) {
        os << glm::to_string(vector);
        return os;
    };

}

namespace renderbox {

    inline glm::vec3 dehomogenize(glm::vec4 vector) {
        return glm::vec3(vector) / vector.w;
    };

}

#endif //VOX_VECTOR_H

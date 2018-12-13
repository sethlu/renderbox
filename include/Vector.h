#ifndef RENDERBOX_VECTOR_H
#define RENDERBOX_VECTOR_H


#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

namespace renderbox {

    typedef glm::vec2 vec2;
    typedef glm::vec3 vec3;
    typedef glm::vec4 vec4;
    typedef glm::ivec3 ivec3;
    typedef glm::uvec3 uvec3;

    inline vec3 dehomogenize(vec4 vector) {
        return vec3(vector) / vector.w;
    };

}


template<glm::length_t L, typename T, glm::qualifier Q = glm::defaultp>
std::ostream &operator<<(std::ostream &os, glm::vec<L, T, Q> const &vec) {
    os << glm::to_string(vec);
    return os;
}


#endif //RENDERBOX_VECTOR_H

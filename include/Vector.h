#ifndef VOX_VECTOR_H
#define VOX_VECTOR_H


#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>

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

#endif //VOX_VECTOR_H

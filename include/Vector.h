#ifndef RENDERBOX_VECTOR_H
#define RENDERBOX_VECTOR_H


#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>


namespace renderbox {

    typedef glm::vec2 vec2;
    typedef glm::vec3 vec3;
    typedef glm::vec4 vec4;

    typedef glm::ivec2 ivec2;
    typedef glm::ivec3 ivec3;
    typedef glm::ivec4 ivec4;

    typedef glm::uvec2 uvec2;
    typedef glm::uvec3 uvec3;
    typedef glm::uvec4 uvec4;

    inline vec3 dehomogenize(vec4 vector) {
        return vec3(vector) / vector.w;
    };

}


#endif //RENDERBOX_VECTOR_H

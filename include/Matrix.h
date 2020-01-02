#ifndef RENDERBOX_MATRIX_H
#define RENDERBOX_MATRIX_H


#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>

namespace renderbox {

    typedef glm::mat2 mat2;
    typedef glm::mat3 mat3;
    typedef glm::mat4 mat4;

    using glm::inverse;

}


#endif //RENDERBOX_MATRIX_H

#ifndef VOX_MATRIX_H
#define VOX_MATRIX_H


#include <glm/gtx/string_cast.hpp>

namespace {

    std::ostream &operator<<(std::ostream &os, const glm::mat4x4 &matrix) {
        os << glm::to_string(matrix);
        return os;
    };

}


#endif //VOX_MATRIX_H

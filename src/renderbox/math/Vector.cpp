#include "Vector.h"


namespace renderbox {

    void printVector(glm::vec3 vector) {
        fprintf(stdout, "%.4f\t%.4f\t%.4f\n", vector.x, vector.y, vector.z);
    }

    glm::vec3 dehomogenize(glm::vec4 vector) {
        return glm::vec3(vector) / vector.w;
    }

}
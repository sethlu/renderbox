#include "Geometry.h"


namespace renderbox {

    std::vector<glm::vec3> Geometry::getVertices() {
        return vertices;
    }

    std::vector<glm::uvec3> Geometry::getFaces() {
        return faces;
    }

}
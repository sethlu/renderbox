#include "Geometry.h"


namespace renderbox {

    std::vector<glm::vec3> Geometry::getVertices() const {
        return vertices;
    };

    std::vector<glm::vec3> Geometry::getNormals() const {
        return normals;
    };

    std::vector<glm::uvec3> Geometry::getFaces() const {
        return faces;
    };

}

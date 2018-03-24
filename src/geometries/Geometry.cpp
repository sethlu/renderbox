#include "Vector.h"
#include "Geometry.h"


namespace renderbox {

    std::vector<vec3> &Geometry::getVertices() {
        return vertices;
    };

    std::vector<vec2> &Geometry::getUVs() {
        return uvs;
    };

    std::vector<vec3> &Geometry::getNormals() {
        return normals;
    };

    std::vector<uvec3> &Geometry::getFaces() {
        return faces;
    };

}

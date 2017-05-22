#ifndef PARTICLESIM_GEOMETRY_H
#define PARTICLESIM_GEOMETRY_H


#include <vector>
#include <glm/vec3.hpp>
#include <GL/glew.h>

namespace renderbox {

    class Geometry {
    protected:
        std::vector<glm::vec3> vertices;
        std::vector<glm::uvec3> faces;
    public:
        std::vector<glm::vec3> getVertices();
        std::vector<glm::uvec3> getFaces();
    };

}


#endif //PARTICLESIM_GEOMETRY_H

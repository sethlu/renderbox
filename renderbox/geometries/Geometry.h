#ifndef VOX_GEOMETRY_H
#define VOX_GEOMETRY_H


#include "../platform.h"
#if defined(__MACOSX__)
#include <OpenGL/gl3.h>
#elif defined(__IPHONEOS__)
#include <OpenGLES/ES3/gl.h>
#endif
#include <vector>
#include <glm/vec3.hpp>

namespace renderbox {

    class Geometry {
    protected:
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<glm::uvec3> faces;
    public:
        std::vector<glm::vec3> getVertices();
        std::vector<glm::vec3> getNormals();
        std::vector<glm::uvec3> getFaces();
    };

}


#endif //VOX_GEOMETRY_H
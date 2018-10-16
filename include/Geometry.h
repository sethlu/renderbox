#ifndef RENDERBOX_GEOMETRY_H
#define RENDERBOX_GEOMETRY_H


#include <vector>

#include "Vector.h"

namespace renderbox {

    class Geometry {

        friend class OpenGLRenderer;

        friend class MetalRenderer;

    public:

        std::vector<vec3> &getVertices();

        std::vector<vec2> &getUVs();

        std::vector<vec3> &getNormals();

        std::vector<uvec3> &getFaces();

        void regenerateNormals();

    protected:

        std::vector<vec3> vertices;

        std::vector<vec2> uvs;

        std::vector<vec3> normals;

        std::vector<uvec3> faces;

    };

}


#endif //RENDERBOX_GEOMETRY_H

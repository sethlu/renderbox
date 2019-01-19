#ifndef RENDERBOX_GEOMETRY_H
#define RENDERBOX_GEOMETRY_H


#include <vector>

#include "Vector.h"
#include "VersionTrackedObject.h"

namespace renderbox {

    class Geometry : public VersionTrackedObject {

        friend class OpenGLRenderer;

        friend class MetalRenderer;

    public:

        virtual ~Geometry();

        std::vector<vec3> vertices;

        std::vector<vec2> uvs;

        std::vector<vec3> normals;

        std::vector<uvec3> faces;

        std::vector<uvec4> skinIndices;

        std::vector<vec4> skinWeights;

        void regenerateNormals();

    };

}


#endif //RENDERBOX_GEOMETRY_H

#ifndef RENDERBOX_MESH_GEOMETRY_H
#define RENDERBOX_MESH_GEOMETRY_H


#include "Geometry.h"

#include <vector>


namespace renderbox {

    class MeshGeometry : public Geometry {
    public:

        ~MeshGeometry() override = default;

        std::vector<vec3> vertices;

        std::vector<vec2> uvs;

        std::vector<vec3> normals;

        std::vector<uvec3> faces;

        std::vector<uvec4> skinIndices;

        std::vector<vec4> skinWeights;

        void regenerateNormals();

    };

}


#endif //RENDERBOX_MESH_GEOMETRY_H

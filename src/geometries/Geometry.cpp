#include "Vector.h"
#include "Geometry.h"


namespace renderbox {

    Geometry::~Geometry() = default;

    std::vector<vec3> &Geometry::getVertices() {
        return vertices;
    }

    std::vector<vec2> &Geometry::getUVs() {
        return uvs;
    }

    std::vector<vec3> &Geometry::getNormals() {
        return normals;
    }

    std::vector<uvec3> &Geometry::getFaces() {
        return faces;
    }

    void Geometry::regenerateNormals() {
        auto const numVertices = vertices.size();

        normals.resize(numVertices);
        std::vector<unsigned short> numFaces(numVertices);

        for (auto const& face : faces) {
            auto faceNormal = glm::cross(
                    glm::normalize(vertices[face[1]] - vertices[face[0]]),
                    glm::normalize(vertices[face[2]] - vertices[face[0]]));
            normals[face[0]] = faceNormal;
            normals[face[1]] = faceNormal;
            normals[face[2]] = faceNormal;
            numFaces[face[0]]++;
            numFaces[face[1]]++;
            numFaces[face[2]]++;
        }

        for (auto i = 0; i < numVertices; i++) {
            normals[i] /= numFaces[i];
        }
    }

}

#include "MeshGeometry.h"


namespace renderbox {

    void MeshGeometry::regenerateNormals() {
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

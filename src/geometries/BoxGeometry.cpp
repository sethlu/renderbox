#include "BoxGeometry.h"

#include <glm/geometric.hpp>

#include "Vector.h"


namespace renderbox {

    BoxGeometry::BoxGeometry(float width, float height, float depth) {
        float width2 = width / 2, height2 = height / 2, depth2 = depth / 2;

        vertices.push_back(vec3(width2, height2, depth2));
        vertices.push_back(vec3(-width2, height2, depth2));
        vertices.push_back(vec3(-width2, -height2, depth2));
        vertices.push_back(vec3(width2, -height2, depth2));
        vertices.push_back(vec3(width2, height2, -depth2));
        vertices.push_back(vec3(-width2, height2, -depth2));
        vertices.push_back(vec3(-width2, -height2, -depth2));
        vertices.push_back(vec3(width2, -height2, -depth2));

        normals.push_back(glm::normalize(vec3(1, 1, 1)));
        normals.push_back(glm::normalize(vec3(-1, 1, 1)));
        normals.push_back(glm::normalize(vec3(-1, -1, 1)));
        normals.push_back(glm::normalize(vec3(1, -1, 1)));
        normals.push_back(glm::normalize(vec3(1, 1, -1)));
        normals.push_back(glm::normalize(vec3(-1, 1, -1)));
        normals.push_back(glm::normalize(vec3(-1, -1, -1)));
        normals.push_back(glm::normalize(vec3(1, -1, -1)));

        faces.push_back(uvec3(0, 1, 2));
        faces.push_back(uvec3(2, 3, 0));

        faces.push_back(uvec3(3, 2, 6));
        faces.push_back(uvec3(6, 7, 3));

        faces.push_back(uvec3(7, 6, 5));
        faces.push_back(uvec3(5, 4, 7));

        faces.push_back(uvec3(4, 5, 1));
        faces.push_back(uvec3(1, 0, 4));

        faces.push_back(uvec3(4, 0, 3));
        faces.push_back(uvec3(3, 7, 4));

        faces.push_back(uvec3(1, 5, 6));
        faces.push_back(uvec3(6, 2, 1));

    }

}

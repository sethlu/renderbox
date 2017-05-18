#include "BoxGeometry.h"


namespace renderbox {

    BoxGeometry::BoxGeometry(float width, float height, float depth) {
        float width2 = width / 2, height2 = height / 2, depth2 = depth / 2;

        vertices.push_back(glm::vec3(width2, height2, depth2));
        vertices.push_back(glm::vec3(-width2, height2, depth2));
        vertices.push_back(glm::vec3(-width2, -height2, depth2));
        vertices.push_back(glm::vec3(width2, -height2, depth2));
        vertices.push_back(glm::vec3(width2, height2, -depth2));
        vertices.push_back(glm::vec3(-width2, height2, -depth2));
        vertices.push_back(glm::vec3(-width2, -height2, -depth2));
        vertices.push_back(glm::vec3(width2, -height2, -depth2));

        faces.push_back(glm::vec3(0, 1, 2));
        faces.push_back(glm::vec3(2, 3, 0));

        faces.push_back(glm::vec3(3, 2, 6));
        faces.push_back(glm::vec3(6, 7, 3));

        faces.push_back(glm::vec3(7, 6, 5));
        faces.push_back(glm::vec3(5, 4, 7));

        faces.push_back(glm::vec3(4, 5, 1));
        faces.push_back(glm::vec3(1, 0, 4));

        faces.push_back(glm::vec3(4, 0, 3));
        faces.push_back(glm::vec3(3, 7, 4));

        faces.push_back(glm::vec3(1, 5, 6));
        faces.push_back(glm::vec3(6, 2, 1));

    }

}
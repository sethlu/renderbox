#include <iostream>
#include <glm/geometric.hpp>
#include <glm/gtx/intersect.hpp>
#include "Ray.h"
#include "Vector.h"


namespace renderbox {

    Ray::Ray(glm::vec3 origin, glm::vec3 direction) {
        this->origin = origin;
        this->direction = direction;
    }

    Ray *Ray::copy(glm::mat4x4 changeOfCoordinates) {
        glm::vec4 homogeneousOrigin = changeOfCoordinates * glm::vec4(origin, 1.0f);
        glm::vec4 homogeneousTestVector = changeOfCoordinates * glm::vec4(origin + direction, 1.0f);
        glm::vec3 copyOrigin = dehomogenize(homogeneousOrigin);
        glm::vec3 copyDirection = glm::normalize(dehomogenize(homogeneousTestVector)
                                                 - dehomogenize(homogeneousOrigin));
        return new Ray(copyOrigin, copyDirection);
    }

    glm::vec3 Ray::getOrigin() const {
        return origin;
    }

    glm::vec3 Ray::getDirection() const {
        return direction;
    }

    bool Ray::intersectTriangle(glm::vec3 &v0, glm::vec3 &v1, glm::vec3 &v2, glm::vec3 &baryPosition) {
        return glm::intersectRayTriangle(origin, direction, v0, v1, v2, baryPosition);
    }

    bool Ray::intersectObject(Object *object, std::vector<glm::vec3> &positions) {

        // Calculate ray in object coordinates

        glm::mat4x4 objectWorldMatrix = object->getWorldMatrix();
        Ray *objectCoordRay = copy(glm::inverse(objectWorldMatrix));

        // Object geometry

        int count = 0;

        std::vector<glm::vec3> vertices = object->getGeometry()->getVertices();
        std::vector<glm::uvec3> faces = object->getGeometry()->getFaces();

        std::vector<float> distances;

        positions.clear();
        for (glm::uvec3 face : faces) {
            glm::vec3 intersectionBaryPosition;
            if (objectCoordRay->intersectTriangle(vertices[face[0]],
                                                  vertices[face[1]],
                                                  vertices[face[2]],
                                                  intersectionBaryPosition)) {
                distances.push_back(intersectionBaryPosition.z);
                ++count;
            }
        }

        if (count > 0) {
            // Sort all distances
            std::sort(distances.begin(), distances.end());
            for (float distance : distances) {
                positions.push_back(origin + direction * distance);
            }
            return true;
        }
        return false;

    }

}
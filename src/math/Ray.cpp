#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include <glm/geometric.hpp>
#include <glm/gtx/intersect.hpp>
#include "Ray.h"


namespace renderbox {

    Ray::Ray(vec3 origin, vec3 direction) {
        this->origin = origin;
        this->direction = direction;
    }

    Ray *Ray::copy(mat4 changeOfCoordinates) {
        vec4 homogeneousOrigin = changeOfCoordinates * vec4(origin, 1.0f);
        vec4 homogeneousTestVector = changeOfCoordinates * vec4(origin + direction, 1.0f);
        vec3 copyOrigin = dehomogenize(homogeneousOrigin);
        vec3 copyDirection = glm::normalize(dehomogenize(homogeneousTestVector)
                                                 - dehomogenize(homogeneousOrigin));
        return new Ray(copyOrigin, copyDirection);
    }

    vec3 Ray::getOrigin() const {
        return origin;
    }

    vec3 Ray::getDirection() const {
        return direction;
    }

    bool Ray::intersectTriangle(vec3 &v0, vec3 &v1, vec3 &v2, vec2 &baryPosition, float &distance) {
        return glm::intersectRayTriangle(origin, direction, v0, v1, v2, baryPosition, distance);
    }

    bool Ray::intersectObject(Object *object, std::vector<vec3> &positions) {

        // Calculate ray in object coordinates

        mat4 objectWorldMatrix = object->getWorldMatrix();
        Ray *objectCoordRay = copy(glm::inverse(objectWorldMatrix));

        // Object geometry

        int count = 0;

        auto &vertices = object->getGeometry()->getVertices();
        auto &faces = object->getGeometry()->getFaces();

        std::vector<float> distances;

        positions.clear();
        for (uvec3 face : faces) {
            vec2 intersectionBaryPosition;
			float intersectionDistance;
            if (objectCoordRay->intersectTriangle(vertices[face[0]],
                                                  vertices[face[1]],
                                                  vertices[face[2]],
                                                  intersectionBaryPosition,
												  intersectionDistance)) {
                distances.push_back(intersectionDistance);
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

#include "Ray.h"

#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/geometric.hpp>
#include <glm/gtx/intersect.hpp>


namespace renderbox {

    Ray::Ray(vec3 origin, vec3 direction) {
        this->origin = origin;
        this->direction = direction;
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
        Ray objectCoordRay = glm::inverse(objectWorldMatrix) * *this;

        // Object geometry

        int count = 0;

        auto &vertices = object->getGeometry()->vertices;
        auto &faces = object->getGeometry()->faces;

        std::vector<float> distances;

        positions.clear();
        for (uvec3 face : faces) {
            vec2 intersectionBaryPosition;
			float intersectionDistance;
            if (objectCoordRay.intersectTriangle(
                    vertices[face[0]],
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
                positions.emplace_back(dehomogenize(objectWorldMatrix * vec4(objectCoordRay.origin + objectCoordRay.direction * distance, 1)));
            }
            return true;
        }
        return false;

    }

    Ray operator*(const mat4 &matrix, const Ray &ray) {
        vec4 homogeneousOrigin = matrix * vec4(ray.origin, 1.0f);
        vec4 homogeneousTestVector = matrix * vec4(ray.origin + ray.direction, 1.0f);
        vec3 copyOrigin = dehomogenize(homogeneousOrigin);
        vec3 copyDirection = glm::normalize(dehomogenize(homogeneousTestVector)
                                            - dehomogenize(homogeneousOrigin));
        return {copyOrigin, copyDirection};
    }

}

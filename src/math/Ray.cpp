#include "Ray.h"

#include <iostream>
#include <algorithm>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/geometric.hpp>
#include <glm/gtx/intersect.hpp>

#include "MeshGeometry.h"


namespace renderbox {

    Ray::Ray(vec3 const &origin, vec3 const &direction)
        : origin(origin), direction(direction) {

    }

    vec3 Ray::getOrigin() const {
        return origin;
    }

    vec3 Ray::getDirection() const {
        return direction;
    }

    bool Ray::intersectTriangle(const vec3 &v0, const vec3 &v1, const vec3 &v2, vec2 &baryPosition, float &distance) const {
        return glm::intersectRayTriangle(origin, direction, v0, v1, v2, baryPosition, distance);
    }

    bool Ray::intersectLine(vec3 const &lineOrigin, vec3 const &lineDirection, float epsilon, float &distance, float &rt, float &lt) const {
        vec2 ts = inverse(mat2(
                dot(direction, direction), - dot(lineDirection, direction),
                - dot(direction, lineDirection), dot(lineDirection, lineDirection))) *
                   vec2(dot(direction, - origin + lineOrigin),
                        - dot(lineDirection, - origin + lineOrigin));

        rt = ts[0];
        lt = ts[1];

        auto pr = origin + rt * direction;
        auto pl = lineOrigin + lt * lineDirection;
        distance = renderbox::distance(pl, pr);

        return rt >= 0 && distance <= epsilon;
    }

    bool Ray::intersectLineSegment(vec3 const &a, vec3 const &b, float epsilon, float &distance, float &rt, float &lt) const {
        auto ab = b - a;
        return intersectLine(a, normalize(ab), epsilon, distance, rt, lt) && lt >= 0 && lt <= length(ab);
    }

    bool Ray::intersectCurve(Curve const *curve, float epsilon, std::vector<vec3> &positions, float interval) const {
        auto const &points = curve->getPointsWithInterval(interval);
        size_t numPoints = points.size();

        std::vector<float> rts;

        float tempDistance, tempRt, tempLt;
        for (size_t i = 0; i < numPoints - 1; i++) {
            if (intersectLineSegment(points[i], points[i + 1], epsilon, tempDistance, tempRt, tempLt)) {
                rts.emplace_back(tempRt);
            }
        }

        positions.clear();
        if (!rts.empty()) {
            std::sort(rts.begin(), rts.end());
            for (float rt : rts) {
                positions.emplace_back(origin + rt * direction);
            }
            return true;
        }
        return false;
    }

    bool Ray::intersectObject(Object const *object, std::vector<vec3> &positions) const {
        auto geometry = dynamic_cast<MeshGeometry *>(object->getGeometry().get());
        if (!geometry) return false;

        // Calculate ray in object coordinates

        mat4 objectWorldMatrix = object->getWorldMatrix();
        Ray objectCoordRay = glm::inverse(objectWorldMatrix) * *this;

        // Object geometry

        int count = 0;

        auto &vertices = geometry->vertices;
        auto &faces = geometry->faces;

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

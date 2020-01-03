#include "Ray.h"

#include <iostream>
#include <algorithm>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/geometric.hpp>
#include <glm/gtx/intersect.hpp>

#include "MeshGeometry.h"
#include "CurveGeometry.h"


namespace renderbox {

    template<typename A, typename B>
    static bool compFirstAsc(std::pair<A, B> const &a, std::pair<A, B> const &b) {
        return a.first < b.first;
    }

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

    bool Ray::intersectSphere(vec3 const &center, float radius, float &distance) const {
        return glm::intersectRaySphere(origin, direction, center, radius * radius, distance);
    }

    bool Ray::intersectPoint(vec3 const &position, float epsilon, float &errorDistance, float &rt) const {
        auto op = position - origin;
        rt = dot(op, direction);
        errorDistance = length(op - rt * direction);
        return errorDistance <= epsilon;
    }

    bool Ray::intersectLine(vec3 const &lineOrigin, vec3 const &lineDirection, float epsilon, float &errorDistance, float &rt, float &lt) const {
        vec2 ts = inverse(mat2(
                dot(direction, direction), - dot(lineDirection, direction),
                - dot(direction, lineDirection), dot(lineDirection, lineDirection))) *
                   vec2(dot(direction, - origin + lineOrigin),
                        - dot(lineDirection, - origin + lineOrigin));

        rt = ts[0];
        lt = ts[1];

        auto pr = origin + rt * direction;
        auto pl = lineOrigin + lt * lineDirection;
        errorDistance = renderbox::distance(pl, pr);

        return rt >= 0 && errorDistance <= epsilon;
    }

    bool Ray::intersectLineSegment(vec3 const &a, vec3 const &b, float epsilon, float &errorDistance, float &rt, float &lt,
                                   bool capsule) const {
        auto ab = b - a;
        auto abLength = length(ab);
        if (intersectLine(a, normalize(ab), epsilon, errorDistance, rt, lt)) {
            if (lt >= 0 && lt <= abLength) {
                return true;
            }
            if (capsule) {
                if (intersectPoint(a, epsilon, errorDistance, rt)) {
                    lt = 0;
                    return true;
                }
                if (intersectPoint(b, epsilon, errorDistance, rt)) {
                    lt = abLength;
                    return true;
                }
            }
        }
        return false;
    }

    bool Ray::intersectCurve(Curve const *curve, float epsilon, std::vector<vec3> &positions, float interval) const {
        auto const &points = curve->getPointsWithInterval(interval);
        size_t numPoints = points.size();

        std::vector<std::pair<float, vec3>> ts;

        float tempDistance, tempRt, tempLt;
        for (size_t i = 0; i < numPoints - 1; i++) {
            auto const &a = points[i];
            auto const &b = points[i + 1];
            if (intersectLineSegment(a, b, epsilon, tempDistance, tempRt, tempLt)) {
                ts.emplace_back(std::make_pair(tempDistance, a + normalize(b - a) * tempLt));
            }
        }

        positions.clear();
        if (!ts.empty()) {
            std::sort(ts.begin(), ts.end(), compFirstAsc<float, vec3>);
            for (auto const &pair : ts) {
                positions.emplace_back(pair.second);
            }
            return true;
        }
        return false;
    }

    bool Ray::intersectObject(Object const *object, std::vector<vec3> &positions) const {
        auto meshGeometry = dynamic_cast<MeshGeometry *>(object->getGeometry().get());
        if (!meshGeometry) return false;

        // Calculate ray in object coordinates

        mat4 objectWorldMatrix = object->getWorldMatrix();
        Ray objectCoordRay = glm::inverse(objectWorldMatrix) * *this;

        // Object geometry

        auto &vertices = meshGeometry->vertices;
        auto &faces = meshGeometry->faces;

        std::vector<float> rayDistances;

        for (uvec3 face : faces) {
            vec2 intersectionBaryPosition;
			float intersectionDistance;
            if (objectCoordRay.intersectTriangle(
                    vertices[face[0]],
                    vertices[face[1]],
                    vertices[face[2]],
                    intersectionBaryPosition,
                    intersectionDistance)) {
                rayDistances.emplace_back(intersectionDistance);
            }
        }

        positions.clear();
        if (!rayDistances.empty()) {
            // Sort all distances
            std::sort(rayDistances.begin(), rayDistances.end());
            for (float distance : rayDistances) {
                positions.emplace_back(dehomogenize(objectWorldMatrix * vec4(objectCoordRay.origin + objectCoordRay.direction * distance, 1)));
            }
            return true;
        }
        return false;

    }

    bool Ray::intersectObjectWithCurveGeometry(const Object *object, float epsilon, vec3 &closestPosition) const {
        auto curveGeometry = dynamic_cast<CurveGeometry *>(object->getGeometry().get());
        if (!curveGeometry) return false;

        // Calculate ray in object coordinates

        mat4 objectWorldMatrix = object->getWorldMatrix();
        Ray objectCoordRay = glm::inverse(objectWorldMatrix) * *this;

        // Object geometry

        auto const &cachedPointsAndTangents = curveGeometry->cachedPointsAndTangents;
        size_t numPoints = cachedPointsAndTangents.size();

        std::vector<std::pair<float, vec3>> ts;

        float tempDistance, tempRt, tempLt;
        for (size_t i = 0; i < numPoints - 1; i++) {
            auto const &a = cachedPointsAndTangents[i].first;
            auto const &b = cachedPointsAndTangents[i + 1].first;
            if (intersectLineSegment(a, b, epsilon, tempDistance, tempRt, tempLt)) {
                ts.emplace_back(std::make_pair(tempDistance, a + normalize(b - a) * tempLt));
            }
        }

        if (!ts.empty()) {
            std::sort(ts.begin(), ts.end(), compFirstAsc<float, vec3>);
            closestPosition = ts[0].second;
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

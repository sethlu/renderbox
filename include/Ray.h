#ifndef RENDERBOX_RAY_H
#define RENDERBOX_RAY_H


#include "Object.h"
#include "Curve.h"

namespace renderbox {

    class Ray {

        friend Ray operator*(const mat4 &matrix, const Ray &ray);

    public:

        Ray(vec3 const &origin, vec3 const &direction);

        vec3 getOrigin() const;

        vec3 getDirection() const;

        bool intersectTriangle(const vec3 &v0, const vec3 &v1, const vec3 &v2, vec2 &baryPosition, float &distance) const;

        /**
         * @param lineOrigin Origin of the line for testing intersection
         * @param lineDirection Direction of the line for testing intersection (normalized)
         * @param epsilon Allowed error distance for intersection
         * @param distance Closest distance between the ray (as a line) and the line
         * @param rt Parameter for closest point on the ray
         * @param lt Parameter for closest point on the line
         * @return If the ray intersects the line
         */
        bool intersectLine(vec3 const &lineOrigin, vec3 const &lineDirection, float epsilon, float &distance, float &rt, float &lt) const;

        /**
         * @param a A point for the line segment
         * @param b Another point for the line segment
         * @param epsilon Allowed error distance for intersection
         * @param distance Closest distance between the ray (as a line) and the line segment
         * @param rt Parameter for closest point on the ray
         * @param lt Parameter for closest point on the line segment
         * @return If the ray intersects the line segment
         */
        bool intersectLineSegment(vec3 const &a, vec3 const &b, float epsilon, float &distance, float &rt, float &lt) const;

        /**
         * Rasterizes curve into mini line segments and tests intersection in each
         *
         * @param curve Curve for testing intersections
         * @param epsilon Allowed error distance for intersection
         * @param positions To which vector to store all intersecting positions (sorted)
         * @param interval Resolution for curve rasterization
         * @return If the ray intersects the curve
         */
        bool intersectCurve(Curve const *curve, float epsilon, std::vector<vec3> &positions, float interval) const;

        bool intersectObject(Object const *object, std::vector<vec3> &positions) const;

        /**
         * A special version of `intersectObject` that deals with curve geometry.
         * Rather than testing intersections with the mesh faces, this method relies on distances to the curve
         * descriptor that defines the geometry.
         *
         * Note: It's more efficient to use this method to repetitively test intersections with curves than
         * `intersectCurve` because the rasterized points are cached in the curve geometry.
         *
         * @param object Object for testing intersection, expected to have a curve geometry
         * @param epsilon Allowed error distance for intersection
         * @param position Closest point on the object's curve descriptor in world coordinates
         * @param t Parameter for the closest point on the ray
         * @return If the ray intersects the object
         */
        bool intersectObjectWithCurveGeometry(const Object *object, float epsilon, vec3 &position, float &t) const;

    protected:

        vec3 origin;

        vec3 direction;
    };

    Ray operator*(const mat4 &matrix, const Ray &ray);

}


#endif //RENDERBOX_RAY_H

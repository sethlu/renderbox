#include "Curve.h"


namespace renderbox {

    CubicHermiteSpline::CubicHermiteSpline(vec3 p0, vec3 v0, vec3 p1, vec3 v1) :
        p0(p0), v0(v0), p1(p1), v1(v1) {

    }

    vec3 CubicHermiteSpline::getPoint(float t) const {
        auto t2 = t * t;
        auto t3 = t2 * t;

        auto c0 = p0;
        auto c1 = v0;
        auto c2 = -3.f * p0 + 3.f * p1 - 2.f * v0 - v1;
        auto c3 = 2.f * p0 - 2.f * p1 + v0 + v1;

        return c3 * t3 + c2 * t2 + c1 * t + c0;
    }

    vec3 CubicHermiteSpline::getTangent(float t) const {
        auto t2 = t * t;

        auto c1 = v0;
        auto c2 = -3.f * p0 + 3.f * p1 - 2.f * v0 - v1;
        auto c3 = 2.f * p0 - 2.f * p1 + v0 + v1;

        return 3.f * c3 * t2 + 2.f * c2 * t + c1;
    }

    /**
     * @return Estimated length of the cubic spline [0, t] with 0 <= t <= 1
     */
    float CubicHermiteSpline::getEstimatedDistance(float t) const {
        struct GaussLegendreQuadratureCoefficient {
            float point;
            float weight;
        };

        static GaussLegendreQuadratureCoefficient coefs[] = {
                {0, 0.568889},
                { 0.5384693, 0.47862867},
                {-0.5384693, 0.47862867},
                {0.90617985, 0.23692688},
                {-0.90617985, 0.23692688},
        };

        auto ht = t / 2;
        return ht * (
                coefs[0].weight * glm::length(getTangent(ht * (coefs[0].point + 1.f))) +
                coefs[1].weight * glm::length(getTangent(ht * (coefs[1].point + 1.f))) +
                coefs[2].weight * glm::length(getTangent(ht * (coefs[2].point + 1.f))) +
                coefs[3].weight * glm::length(getTangent(ht * (coefs[3].point + 1.f))) +
                coefs[4].weight * glm::length(getTangent(ht * (coefs[4].point + 1.f))));
    }

}

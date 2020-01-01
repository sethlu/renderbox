#include "Curve.h"


namespace renderbox {

    CubicSpline::CubicSpline(mat4 m, vec3 p0, vec3 p1, vec3 p2, vec3 p3) :
        m(m), p0(p0), p1(p1), p2(p2), p3(p3) {

    }

    vec3 CubicSpline::getPoint(float t) const {
        auto t2 = t * t;
        auto t3 = t2 * t;
        auto vec_t = vec4(t3, t2, t, 1);

        return glm::dot(vec_t, m[0]) * p0 +
                glm::dot(vec_t, m[1]) * p1 +
                glm::dot(vec_t, m[2]) * p2 +
                glm::dot(vec_t, m[3]) * p3;
    }

    vec3 CubicSpline::getTangent(float t) const {
        auto t2 = t * t;
        auto vec_t = vec4(3 * t2, 2 * t, 1, 0);

        return glm::dot(vec_t, m[0]) * p0 +
               glm::dot(vec_t, m[1]) * p1 +
               glm::dot(vec_t, m[2]) * p2 +
               glm::dot(vec_t, m[3]) * p3;
    }

    /**
     * @return Estimated length of the cubic spline [0, t] with 0 <= t <= 1
     */
    float CubicSpline::getEstimatedDistance(float t) const {
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

    std::pair<std::shared_ptr<Curve>, std::shared_ptr<Curve>> CubicSpline::split(float t) const {
        auto t2 = t * t;
        auto t3 = t2 * t;

        auto tc = 1.f - t;
        auto tc2 = tc * tc;
        auto tc3 = tc2 * tc;

        auto a = mat4(
                t3, 0, 0, 0,
                0, t2, 0, 0,
                0, 0, t, 0,
                0, 0, 0, 1);
        auto ac = mat4(
                tc3, 3 * t * tc2, 3 * t2 * tc, t3,
                0, tc2, 2 * t * tc, t2,
                0, 0, tc, t,
                0, 0, 0, 1
                );

        return std::make_pair(
                std::make_shared<CubicSpline>(a * m, p0, p1, p2, p3),
                std::make_shared<CubicSpline>(ac * m, p0, p1, p2, p3)
                );
    }

}

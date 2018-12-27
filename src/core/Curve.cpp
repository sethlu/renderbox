#include "Curve.h"
#include "logging.h"


namespace renderbox {

    CatmullRomCurve::CatmullRomCurve() = default;

    inline float tj(float ti, vec3 pi, vec3 pj, float alpha) {
        return pow(glm::distance(pi, pj), alpha) + ti;
    }

    vec3 CatmullRomCurve::getPoint(float t) const {
        LOG_ASSERT(0 <= t && t <= 1) << "t = " << t << std::endl;

        auto n = points.size();

        LOG_ASSERT(n >= 1);

        auto numSegments = n - 1;
        auto tPerSegment = 1.f / numSegments;

        int i = floor(t * (n - 1));
        t = (t - i * tPerSegment) / tPerSegment;

        vec3 p0 = points[i - 1 >= 0 ? i - 1 : 0];
        vec3 p1 = points[i];
        vec3 p2 = points[i + 1];
        vec3 p3 = points[i + 2 <= n - 1 ? i + 2 : n - 1];

        // Linear interpolation for first & last control points

        if (i == 0 || i == n - 2) {
            return (1 - t) * p1 + t * p2;
        }

        // Interpolate uniform Catmull-Rom curve [p0, p1, p2, p3] at t

        auto v1 = p2 - p0;
        auto v2 = p3 - p1;

        auto t2 = t * t;
        auto t3 = t2 * t;

        auto c0 = p1;
        auto c1 = v1;
        auto c2 = -3.f * p1 + 3.f * p2 - 2.f * v1 - v2;
        auto c3 = 2.f * p1 - 2.f * p2 + v1 + v2;

        return c3 * t3 + c2 * t2 + c1 * t + c0;
    }

    std::vector<vec3> CatmullRomCurve::getPoints(unsigned divisions) const {
        std::vector<vec3> result;
        if (empty()) return result;

        if (divisions <= 0) {
            divisions = (points.size() - 1) * 4;
        }

        for (int i = 0; i <= divisions; i++) {
            result.emplace_back(getPoint(1.f * i / divisions));
        }

        return result;
    }

    vec3 CatmullRomCurve::getTangent(float t) const {
        return renderbox::vec3();
    }

    bool CatmullRomCurve::empty() const {
        return points.size() <= 1;
    }

}

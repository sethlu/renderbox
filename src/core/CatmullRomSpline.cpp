#include "Curve.h"
#include "logging.h"


namespace renderbox {

    CatmullRomSpline::CatmullRomSpline() = default;

    vec3 CatmullRomSpline::getPoint(float t) const {
        LOG_ASSERT(0 <= t && t <= 1) << "t = " << t << std::endl;
        LOG_ASSERT(!empty());

        auto n = points.size();
        auto numSegments = n - 1;
        auto tPerSegment = 1.f / numSegments;

        int segmentIndex = fmin(numSegments - 1, floor(t * numSegments));
        float segmentT = (t - segmentIndex * tPerSegment) * numSegments;

        vec3 p0 = points[segmentIndex - 1 >= 0 ? segmentIndex - 1 : 0];
        vec3 p1 = points[segmentIndex];
        vec3 p2 = points[segmentIndex + 1];
        vec3 p3 = points[segmentIndex + 2 <= n - 1 ? segmentIndex + 2 : n - 1];
        auto v1 = (p2 - p0) * 0.5f;
        auto v2 = (p3 - p1) * 0.5f;

        return CubicHermiteSpline(p1, v1, p2, v2).getPoint(segmentT);
    }

    vec3 CatmullRomSpline::getTangent(float t) const {
        LOG_ASSERT(0 <= t && t <= 1) << "t = " << t << std::endl;
        LOG_ASSERT(!empty());

        auto n = points.size();
        auto numSegments = n - 1;
        auto tPerSegment = 1.f / numSegments;

        int segmentIndex = fmin(numSegments - 1, floor(t * numSegments));
        float segmentT = (t - segmentIndex * tPerSegment) * numSegments;

        vec3 p0 = points[segmentIndex - 1 >= 0 ? segmentIndex - 1 : 0];
        vec3 p1 = points[segmentIndex];
        vec3 p2 = points[segmentIndex + 1];
        vec3 p3 = points[segmentIndex + 2 <= n - 1 ? segmentIndex + 2 : n - 1];
        auto v1 = (p2 - p0) * 0.5f;
        auto v2 = (p3 - p1) * 0.5f;

        return CubicHermiteSpline(p1, v1, p2, v2).getTangent(segmentT);
    }

    float CatmullRomSpline::getEstimatedDistance(float t) const {
        LOG_ASSERT(!empty());

        auto n = points.size();
        auto numSegments = n - 1;
        auto tPerSegment = 1.f / numSegments;

        int segmentIndex = fmin(numSegments - 1, floor(t * numSegments));
        float segmentT = (t - segmentIndex * tPerSegment) * numSegments;

        float length = 0;

        for (int i = 0; i <= segmentIndex; i++) {
            vec3 p0 = points[i - 1 >= 0 ? i - 1 : 0];
            vec3 p1 = points[i];
            vec3 p2 = points[i + 1];
            vec3 p3 = points[i + 2 <= n - 1 ? i + 2 : n - 1];
            auto v1 = (p2 - p0) * 0.5f;
            auto v2 = (p3 - p1) * 0.5f;

            auto segmentSpline = CubicHermiteSpline(p1, v1, p2, v2);
            float segmentLength = segmentSpline.getEstimatedDistance(i == segmentIndex ? segmentT : 1.f);
            length += segmentLength;
        }

        return length;
    }

    std::vector<vec3> CatmullRomSpline::getPointsWithSegments(unsigned divisions) const {
        return Curve::getPointsWithSegments(divisions > 0 ? divisions : (points.size() - 1) * 4);
    }

    bool CatmullRomSpline::empty() const {
        return points.size() < 2;
    }

    std::pair<std::shared_ptr<Curve>, std::shared_ptr<Curve>> CatmullRomSpline::split(float t) const {
        return std::pair<std::shared_ptr<Curve>, std::shared_ptr<Curve>>();
    }

}

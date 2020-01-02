#include "Curve.h"
#include "logging.h"


namespace renderbox {

    vec3 Curve::getPointAtDistance(
            float distance,
            float hintLowerBoundT,
            float hintUpperBoundT,
            float epsilon,
            float *t) const {
        // Binary search for t such that distance(t) is around searchLength

        float a = hintLowerBoundT;
        float aDistance = getEstimatedDistance(a);

        float b = hintUpperBoundT;
        float bDistance = getEstimatedDistance(b);

        size_t maxIterations = 50;

        while (maxIterations-- > 0 && bDistance - aDistance > epsilon) {
            float mid = (a + b) / 2.f;
            float midDistance = getEstimatedDistance(mid);

            if (distance >= midDistance) {
                a = mid;
                aDistance = midDistance;
            } else {
                b = mid;
                bDistance = midDistance;
            }
        }

        if (maxIterations == 0) {
            LOG(WARNING) << "Ran out of iterations" << std::endl;
        }

        // t = a is a good approximate
        if (t) *t = a;
        return getPoint(a);
    }

    std::vector<vec3> Curve::getPointsWithSegments(unsigned int divisions) const {
        if (empty() || divisions <= 0) return {};

        std::vector<vec3> result;
        for (unsigned int i = 0; i <= divisions; i++) {
            result.emplace_back(getPoint(static_cast<float>(i) / static_cast<float>(divisions)));
        }
        return result;
    }

    std::vector<vec3> Curve::getPointsWithInterval(float interval) const {
        float estimatedTotalLength = getEstimatedDistance(1.f);

        float searchLength = 0;
        float epsilon = 0.001;

        std::vector<vec3> result;

        float prevT = 0;
        while (searchLength < estimatedTotalLength) {
            // Add point to result list
            auto point = getPointAtDistance(searchLength, prevT, 1.f, epsilon, &prevT);
            result.emplace_back(point);
            // Update to find next location
            searchLength += interval;
        }

        return result;
    }

    std::vector<std::pair<vec3, vec3>> Curve::getPointsAndTangentsWithInterval(float interval) const {
        float estimatedTotalLength = getEstimatedDistance(1.f);

        float searchLength = 0;
        float epsilon = 0.001f;

        std::vector<std::pair<vec3, vec3>> result;

        float prevT = 0;
        while (searchLength < estimatedTotalLength) {
            // Add point to result list
            auto point = getPointAtDistance(searchLength, prevT, 1.f, epsilon, &prevT);
            auto tangent = getTangent(prevT);
            result.emplace_back(std::make_pair(point, tangent));
            // Update to find next location
            searchLength += interval;
        }

        return result;
    }

    std::pair<std::shared_ptr<Curve>, std::shared_ptr<Curve>> split(std::shared_ptr<Curve> const &curve, float t) {
        if (auto splittableCurve = dynamic_cast<SplittableCurveInterface *>(curve.get())) {
            return splittableCurve->split(t);
        }
        return std::make_pair(std::make_shared<ParameterMappedCurve>(curve, 0, t),
                              std::make_shared<ParameterMappedCurve>(curve, t, 1));
    }
}

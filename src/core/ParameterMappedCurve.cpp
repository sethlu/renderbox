#include "Curve.h"

#include <utility>
#include "logging.h"


namespace renderbox {

    ParameterMappedCurve::ParameterMappedCurve(std::shared_ptr<Curve> curve, float lmap, float umap)
        : curve(std::move(curve)), lmap(lmap), umap(umap) {

    }

    vec3 ParameterMappedCurve::getPoint(float t) const {
        return curve->getPoint(getMappedParameter(t));
    }

    vec3 ParameterMappedCurve::getTangent(float t) const {
        return curve->getTangent(getMappedParameter(t));
    }

    float ParameterMappedCurve::getEstimatedDistance(float t) const {
        if (t == 0) return 0;
        return curve->getEstimatedDistance(getMappedParameter(t)) -
            curve->getEstimatedDistance(getMappedParameter(0));
    }

    bool ParameterMappedCurve::empty() const {
        return curve->empty();
    }

    std::pair<std::shared_ptr<Curve>, std::shared_ptr<Curve>> ParameterMappedCurve::split(float t) const {
        auto tmap = getMappedParameter(t);
        return std::make_pair(std::make_shared<ParameterMappedCurve>(curve, lmap, tmap),
                              std::make_shared<ParameterMappedCurve>(curve, tmap, umap));
    }

    float ParameterMappedCurve::getMappedParameter(float t) const {
        return lmap + (umap - lmap) * t;
    }

}

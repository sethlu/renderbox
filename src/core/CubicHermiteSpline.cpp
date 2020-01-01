#include "Curve.h"


namespace renderbox {

    CubicHermiteSpline::CubicHermiteSpline(vec3 p0, vec3 v0, vec3 p1, vec3 v1) :
        p0(p0), v0(v0), p1(p1), v1(v1) {

    }

    vec3 CubicHermiteSpline::getPoint(float t) const {
        return toCubicSpline().getPoint(t);
    }

    vec3 CubicHermiteSpline::getTangent(float t) const {
        return toCubicSpline().getTangent(t);
    }

    float CubicHermiteSpline::getEstimatedDistance(float t) const {
        return toCubicSpline().getEstimatedDistance(t);
    }

    std::pair<std::shared_ptr<Curve>, std::shared_ptr<Curve>> CubicHermiteSpline::split(float t) const {
        return toCubicSpline().split(t);
    }

    CubicSpline CubicHermiteSpline::toCubicSpline() const {
        return {{2, -3, 0, 1,
                 -2, 3, 0, 0,
                 1, -2, 1, 0,
                 1, -1, 0, 0},
                p0, p1, v0, v1};
    }

}

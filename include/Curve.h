#ifndef RENDERBOX_CURVE_H
#define RENDERBOX_CURVE_H


#import <vector>

#include "Vector.h"


namespace renderbox {

    class Curve {
    public:

        virtual vec3 getPoint(float t) const = 0;

        virtual vec3 getPointAtDistance(
                float distance,
                float hintLowerBoundT,
                float hintUpperBoundT,
                float epsilon,
                float *t) const;

        virtual vec3 getTangent(float t) const = 0;

        virtual float getEstimatedDistance(float t) const = 0;

        float getEstimatedLength() const {
            return getEstimatedDistance(1.f);
        }

        virtual std::vector<vec3> getPointsWithSegments(unsigned divisions) const;

        virtual std::vector<vec3> getPointsWithInterval(float interval) const;

        virtual std::vector<std::pair<vec3, vec3>> getPointsAndTangentsWithInterval(float interval) const;

        virtual bool empty() const = 0;

    };

    class CubicHermiteSpline : public Curve {
    public:

        explicit CubicHermiteSpline(vec3 p0, vec3 v0, vec3 p1, vec3 v1);

        vec3 getPoint(float t) const override;

        vec3 getTangent(float t) const override;

        float getEstimatedDistance(float t) const override;

        bool empty() const override {
            return false;
        }

        vec3 p0;

        vec3 v0;

        vec3 p1;

        vec3 v1;

    };

    class CatmullRomSpline : public Curve {
    public:

        explicit CatmullRomSpline();

        vec3 getPoint(float t) const override;

        vec3 getTangent(float t) const override;

        float getEstimatedDistance(float t) const override;

        std::vector<vec3> getPointsWithSegments(unsigned divisions) const override;

        bool empty() const override;

        std::vector<vec3> points;

    };

}


#endif //RENDERBOX_CURVE_H

#ifndef RENDERBOX_CURVE_H
#define RENDERBOX_CURVE_H


#import <vector>

#include "Vector.h"


namespace renderbox {

    class Curve {
    public:

        virtual vec3 getPoint(float t) const = 0;

        virtual std::vector<vec3> getPoints(unsigned divisions = 0) const = 0;

        virtual vec3 getTangent(float t) const = 0;

        virtual bool empty() const = 0;

    };

    class CatmullRomCurve : public Curve {
    public:

        explicit CatmullRomCurve();

        vec3 getPoint(float t) const override;

        std::vector<vec3> getPoints(unsigned divisions) const override;

        vec3 getTangent(float t) const override;

        bool empty() const override;

        std::vector<vec3> points;

    };

}


#endif //RENDERBOX_CURVE_H

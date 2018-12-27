#ifndef RENDERBOX_CURVEGEOMETRY_H
#define RENDERBOX_CURVEGEOMETRY_H


#include "Geometry.h"
#include "Curve.h"


namespace renderbox {

    class CurveGeometry : public Geometry {
    public:

        explicit CurveGeometry(std::shared_ptr<Curve> curveDescriptor);

        std::shared_ptr<Curve> curveDescriptor;

        std::vector<vec2> crossSection;

        void updateGeometry();

    };

}


#endif //RENDERBOX_CURVEGEOMETRY_H

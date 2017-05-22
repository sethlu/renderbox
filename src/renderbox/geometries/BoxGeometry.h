#ifndef PARTICLESIM_BOXGEOMETRY_H
#define PARTICLESIM_BOXGEOMETRY_H


#include "Geometry.h"

namespace renderbox {

    class BoxGeometry : public Geometry {
    public:

        BoxGeometry(float width, float height, float depth);
    };

}


#endif //PARTICLESIM_BOXGEOMETRY_H

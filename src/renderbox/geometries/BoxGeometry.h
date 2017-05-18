#include "Geometry.h"


#ifndef PARTICLESIM_BOXGEOMETRY_H
#define PARTICLESIM_BOXGEOMETRY_H


namespace renderbox {

    class BoxGeometry : public Geometry {
    public:

        BoxGeometry(float width, float height, float depth);
    };

}


#endif //PARTICLESIM_BOXGEOMETRY_H

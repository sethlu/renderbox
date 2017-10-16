#ifndef VOX_BOXGEOMETRY_H
#define VOX_BOXGEOMETRY_H


#include "Geometry.h"

namespace renderbox {

    class BoxGeometry : public Geometry {
    public:

        BoxGeometry(float width, float height, float depth);
    };

}


#endif //VOX_BOXGEOMETRY_H

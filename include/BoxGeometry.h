#ifndef RENDERBOX_BOXGEOMETRY_H
#define RENDERBOX_BOXGEOMETRY_H


#include "Geometry.h"

namespace renderbox {

    class BoxGeometry : public Geometry {
    public:

        BoxGeometry(float width, float height, float depth);

    };

}


#endif //RENDERBOX_BOXGEOMETRY_H

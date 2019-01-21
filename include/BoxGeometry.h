#ifndef RENDERBOX_BOXGEOMETRY_H
#define RENDERBOX_BOXGEOMETRY_H


#include "MeshGeometry.h"


namespace renderbox {

    class BoxGeometry : public MeshGeometry {
    public:

        BoxGeometry(float width, float height, float depth);

    };

}


#endif //RENDERBOX_BOXGEOMETRY_H

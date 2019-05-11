#ifndef RENDERBOX_SPHEREGEOMETRY_H
#define RENDERBOX_SPHEREGEOMETRY_H


#include "MeshGeometry.h"


namespace renderbox {

    class SphereGeometry : public MeshGeometry {
    public:

        explicit SphereGeometry(float radius = 1.f,
                                unsigned int widthSegments = 8, unsigned int heightSegments = 6,
                                float phiStart = 0, float phiLength = 2 * M_PI,
                                float thetaStart = 0, float thetaLength = M_PI);

    };

}


#endif //RENDERBOX_SPHEREGEOMETRY_H

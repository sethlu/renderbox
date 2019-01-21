#ifndef RENDERBOX_GEOMETRY_H
#define RENDERBOX_GEOMETRY_H


#include "VersionTrackedObject.h"

#include "Vector.h"


namespace renderbox {

    class Geometry : public VersionTrackedObject {
    public:

        virtual ~Geometry() = default;

    };

}


#endif //RENDERBOX_GEOMETRY_H

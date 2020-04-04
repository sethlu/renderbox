#ifndef RENDERBOX_GEOMETRY_H
#define RENDERBOX_GEOMETRY_H


#include "VersionTrackedObject.h"

#include "Vector.h"


namespace renderbox {

    class Geometry : public VersionTrackedObject {
    public:

        typedef unsigned int id_type;

        virtual ~Geometry() = default;

        inline id_type getGeometryId() const {
            return geometryId;
        }

    protected:

        static unsigned int count;

        id_type geometryId = ++count;

    };

}


#endif //RENDERBOX_GEOMETRY_H

#ifndef RENDERBOX_OPENGLOBJECTPROPERTIES_H
#define RENDERBOX_OPENGLOBJECTPROPERTIES_H


#include "VersionTrackedObject.h"
#include "OpenGLProgram.h"


namespace renderbox {

    struct OpenGLObjectProperties {
        GLuint openglProgramId;
        Geometry::id_type geometryId;
        VersionTrackedObject::version_type geometryVersion;
    };

}

#endif //RENDERBOX_OPENGLOBJECTPROPERTIES_H

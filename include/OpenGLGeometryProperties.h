#ifndef RENDERBOX_OPENGLGEOMETRYPROPERTIES_H
#define RENDERBOX_OPENGLGEOMETRYPROPERTIES_H


#include <unordered_map>
#include <memory>

#include "VersionTrackedObject.h"
#include "OpenGLBuffer.h"
#include "OpenGLVertexArray.h"
#include "ObjectProperties.h"


namespace renderbox {

    class OpenGLGeometryProperties {
    public:

        OpenGLBuffer *getBuffer(int index);

        OpenGLVertexArray *getVertexArray(int index);

        VersionTrackedObject::version_type geometryVersion;

    protected:

        std::unordered_map<int, std::unique_ptr<OpenGLBuffer>> buffers;

        std::unordered_map<int, std::unique_ptr<OpenGLVertexArray>> vertexArrays;

    };

}


#endif //RENDERBOX_OPENGLGEOMETRYPROPERTIES_H

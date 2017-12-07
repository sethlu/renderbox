#ifndef VOX_OPENGLOBJECTPROPERTIES_H
#define VOX_OPENGLOBJECTPROPERTIES_H


#include "platform.h"
#if defined(__MACOSX__)
#include <OpenGL/OpenGL.h>
#elif defined(__IPHONEOS__)
#include <OpenGLES/ES3/gl.h>
#endif
#include <string>
#include <unordered_map>
#include "OpenGLBuffer.h"
#include "OpenGLVertexArray.h"
#include "OpenGLProgram.h"
#include "Material.h"
#include "ObjectProperties.h"

namespace renderbox {

    class OpenGLObjectProperties : public ObjectProperties {

        std::unordered_map<int, std::unique_ptr<OpenGLBuffer>> buffers;

        std::unordered_map<int, std::unique_ptr<OpenGLVertexArray>> vertexArrays;

    public:

        OpenGLBuffer *getBuffer(int index);

        OpenGLVertexArray *getVertexArray(int index);

    };

}


#endif //VOX_OPENGLOBJECTPROPERTIES_H

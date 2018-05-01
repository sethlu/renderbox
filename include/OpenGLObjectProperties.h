#ifndef VOX_OPENGLOBJECTPROPERTIES_H
#define VOX_OPENGLOBJECTPROPERTIES_H


#include "platform.h"
#if defined(__MACOSX__)
#include <OpenGL/gl3.h>
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
#include "OpenGLTexture.h"

namespace renderbox {

    class OpenGLObjectProperties : public ObjectProperties {
    public:

        OpenGLBuffer *getBuffer(int index);

        OpenGLVertexArray *getVertexArray(int index);

        OpenGLTexture *getTexture(int index, bool *blankTexture = nullptr);

    protected:

        std::unordered_map<int, std::unique_ptr<OpenGLBuffer>> buffers;

        std::unordered_map<int, std::unique_ptr<OpenGLVertexArray>> vertexArrays;

        std::unordered_map<int, std::unique_ptr<OpenGLTexture>> textures;

    };

}


#endif //VOX_OPENGLOBJECTPROPERTIES_H

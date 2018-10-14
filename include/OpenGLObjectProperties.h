#ifndef RENDERBOX_OPENGLOBJECTPROPERTIES_H
#define RENDERBOX_OPENGLOBJECTPROPERTIES_H


#include <string>
#include <unordered_map>

#include "OpenGL.h"
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


#endif //RENDERBOX_OPENGLOBJECTPROPERTIES_H

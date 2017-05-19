#include <string>
#include <GL/glew.h>
#include <unordered_map>
#include "OpenGLBuffer.h"
#include "OpenGLVertexArray.h"
#include "OpenGLProgram.h"
#include "../../materials/Material.h"
#include "../ObjectProperties.h"


#ifndef PARTICLESIM_OPENGLBUFFERS_H
#define PARTICLESIM_OPENGLBUFFERS_H


namespace renderbox {

    class OpenGLObjectProperties : public ObjectProperties {
        std::unordered_map<int, OpenGLBuffer *> buffers;
        std::unordered_map<int, OpenGLVertexArray *> vertexArrays;
        static std::unordered_map<int, OpenGLProgram *> programs;
    public:
        OpenGLBuffer *getBuffer(int index);
        OpenGLVertexArray *getVertexArray(int index);
        static OpenGLProgram *getProgram(Material *material);
    };

}


#endif //PARTICLESIM_OPENGLBUFFERS_H

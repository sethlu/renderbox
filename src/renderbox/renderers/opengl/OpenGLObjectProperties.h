#include <string>
#include <GL/glew.h>
#include <map>
#include "OpenGLBuffer.h"
#include "OpenGLVertexArray.h"
#include "OpenGLProgram.h"
#include "../../materials/Material.h"
#include "../ObjectProperties.h"


#ifndef PARTICLESIM_OPENGLBUFFERS_H
#define PARTICLESIM_OPENGLBUFFERS_H


namespace renderbox {

    class OpenGLObjectProperties : public ObjectProperties {
        std::map<int, OpenGLBuffer *> buffers;
        std::map<int, OpenGLVertexArray *> vertexArrays;
        static std::map<int, OpenGLProgram *> programs;
    public:
        OpenGLBuffer *getBuffer(int index);
        OpenGLVertexArray *getVertexArray(int index);
        static OpenGLProgram *getProgram(Material *material);
    };

}


#endif //PARTICLESIM_OPENGLBUFFERS_H

#include <GL/glew.h>
#include <glm/mat4x4.hpp>
#include "OpenGLBuffer.h"


#ifndef PARTICLESIM_OPENGLPROGRAM_H
#define PARTICLESIM_OPENGLPROGRAM_H


namespace renderbox {

    class OpenGLProgram {
        GLuint programID;
    public:
        OpenGLProgram(const char *vertexShaderSource,
                      const char *fragmentShaderSource);
        GLuint getProgramID();
        void useProgram();
        static void stopProgram();
        GLint getAttributeLocation(const char *name);
        void setUniform(const char *name, glm::mat4x4 &matrix);
    };

    std::string readFile(const char *name);

}


#endif //PARTICLESIM_OPENGLPROGRAM_H

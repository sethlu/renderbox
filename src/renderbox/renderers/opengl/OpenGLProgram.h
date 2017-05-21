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
        OpenGLProgram(std::string vertexShaderSource,
                      std::string fragmentShaderSource);
        GLuint getProgramID();
        void useProgram();
        static void stopProgram();
        GLint getAttributeLocation(const char *name);
        void setUniform(const char *name, glm::mat4x4 &matrix);
        void setUniform(const char *name, glm::vec3 &vector);
    };

    std::string readFile(const char *name);

}


#endif //PARTICLESIM_OPENGLPROGRAM_H

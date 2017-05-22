#include <GL/glew.h>
#include <glm/mat4x4.hpp>
#include <unordered_map>
#include "OpenGLBuffer.h"


#ifndef PARTICLESIM_OPENGLPROGRAM_H
#define PARTICLESIM_OPENGLPROGRAM_H


namespace renderbox {

    class OpenGLProgram {

        static std::unordered_map<GLuint, OpenGLProgram *> programs;

        GLuint programID;

    public:

        static OpenGLProgram *getProgram(GLuint programID);

        OpenGLProgram(const char *vertexShaderSource,
                      const char *fragmentShaderSource);
        OpenGLProgram(std::string vertexShaderSource,
                      std::string fragmentShaderSource);

        GLuint getProgramID();

        static void useProgram(GLuint programID);
        void useProgram();
        static void stopProgram();

        GLint getAttributeLocation(const char *name);
        GLint getUniformLocation(const char *name);

    };

    std::string readFile(const char *name);

}


#endif //PARTICLESIM_OPENGLPROGRAM_H

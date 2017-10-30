#ifndef VOX_OPENGLPROGRAM_H
#define VOX_OPENGLPROGRAM_H


#include "platform.h"
#if defined(__MACOSX__)
#include <OpenGL/OpenGL.h>
#elif defined(__IPHONEOS__)
#include <OpenGLES/ES3/gl.h>
#endif
#include <glm/mat4x4.hpp>
#include <unordered_map>
#include "OpenGLBuffer.h"

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


#endif //VOX_OPENGLPROGRAM_H

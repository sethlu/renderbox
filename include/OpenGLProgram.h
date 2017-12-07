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
    public:

        OpenGLProgram(const char *vertexShaderSource,
                      const char *fragmentShaderSource);

        OpenGLProgram(std::string &vertexShaderSource,
                      std::string &fragmentShaderSource);

        static OpenGLProgram *createProgramWithPreprocessedSources(const char *vertexShaderSource,
                                                                   const char *fragmentShaderSource);

        static OpenGLProgram *createProgramWithSourceFiles(const char *vertexShaderFilename,
                                                           const char *fragmentShaderFilename);

        static OpenGLProgram *createProgramWithPreprocessedSourceFiles(const char *vertexShaderFilename,
                                                                       const char *fragmentShaderFilename);

        static OpenGLProgram *getProgram(GLuint programId);

        GLuint getProgramId() const;

        static void useProgram(GLuint programId);

        void useProgram();

        static void stopProgram();

        GLint getAttributeLocation(const char *name);

        GLint getUniformLocation(const char *name);

    protected:

        static std::unordered_map<GLuint, OpenGLProgram *> programs;

        GLuint programId;

    };

    class OpenGLShader {

        friend class OpenGLProgram;

    public:

        OpenGLShader(const char *source, GLenum type);

        ~OpenGLShader();

    protected:

        GLuint shaderId;

    };

}


#endif //VOX_OPENGLPROGRAM_H

#ifndef VOX_OPENGLBUFFER_H
#define VOX_OPENGLBUFFER_H


#include "platform.h"
#if defined(__MACOSX__)
#include <OpenGL/gl3.h>
#elif defined(__IPHONEOS__)
#include <OpenGLES/ES3/gl.h>
#endif
#include <vector>
#include <glm/vec3.hpp>


namespace renderbox {

    class OpenGLBuffer {
    protected:

        GLuint bufferId;

    public:

        OpenGLBuffer();

        GLuint getBufferId() const;

        void bindBuffer(GLenum target = GL_ARRAY_BUFFER);

        static void unbindBuffer(GLenum target = GL_ARRAY_BUFFER);

        void buffer(const void *data, GLsizei size, GLenum usage = GL_STATIC_DRAW);

        void buffer(const std::vector<glm::vec3> &data, GLenum usage = GL_STATIC_DRAW);

        void buffer(const std::vector<glm::uvec3> &data, GLenum usage = GL_STATIC_DRAW);

    };

}


#endif //VOX_OPENGLBUFFER_H

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
#include <glm/vec2.hpp>


namespace renderbox {

    class OpenGLBuffer {
    public:

        OpenGLBuffer();

        virtual ~OpenGLBuffer();

        GLuint getBufferId() const {
            return bufferId;
        }

        void bindBuffer(GLenum target = GL_ARRAY_BUFFER);

        static void unbindBuffer(GLenum target = GL_ARRAY_BUFFER);

        void buffer(const void *data, GLsizei size, GLenum usage = GL_STATIC_DRAW);

        void buffer(const std::vector<glm::vec3> &data, GLenum usage = GL_STATIC_DRAW);

        void buffer(const std::vector<glm::uvec3> &data, GLenum usage = GL_STATIC_DRAW);

        void buffer(const std::vector<glm::vec2> &data, GLenum usage = GL_STATIC_DRAW);

    protected:

        GLuint bufferId;

    };

}


#endif //VOX_OPENGLBUFFER_H

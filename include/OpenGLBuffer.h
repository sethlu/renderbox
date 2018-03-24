#ifndef VOX_OPENGLBUFFER_H
#define VOX_OPENGLBUFFER_H


#include "platform.h"
#if defined(__MACOSX__)
#include <OpenGL/gl3.h>
#elif defined(__IPHONEOS__)
#include <OpenGLES/ES3/gl.h>
#endif
#include <vector>
#include "Vector.h"


namespace renderbox {

    class OpenGLBuffer {

        friend class OpenGLRenderer;

    public:

        OpenGLBuffer();

        virtual ~OpenGLBuffer();

        GLuint getBufferId() const {
            return bufferId;
        }

        void bindBuffer(GLenum target = GL_ARRAY_BUFFER);

        static void unbindBuffer(GLenum target = GL_ARRAY_BUFFER);

        void buffer(const void *data, GLsizei size, GLenum usage = GL_STATIC_DRAW);

        void buffer(const std::vector<vec3> &data, GLenum usage = GL_STATIC_DRAW);

        void buffer(const std::vector<uvec3> &data, GLenum usage = GL_STATIC_DRAW);

        void buffer(const std::vector<vec2> &data, GLenum usage = GL_STATIC_DRAW);

        GLsizei getSize() { return size; }

    protected:

        GLuint bufferId;

        /**
         * @var Size of the data buffered
         */
        GLsizei size;

    };

}


#endif //VOX_OPENGLBUFFER_H

#ifndef RENDERBOX_OPENGLBUFFER_H
#define RENDERBOX_OPENGLBUFFER_H


#include <vector>

#include "OpenGL.h"
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

        template<typename T>
        void buffer(const std::vector<T> &data, GLenum usage = GL_STATIC_DRAW) {
            buffer(&data[0], (GLsizei) data.size() * sizeof(T), usage);
        }

        GLsizei getSize() { return size; }

    protected:

        GLuint bufferId;

        /**
         * @var Size of the data buffered
         */
        GLsizei size;

    };

}


#endif //RENDERBOX_OPENGLBUFFER_H

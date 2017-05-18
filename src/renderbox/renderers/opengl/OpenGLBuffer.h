#include <GL/glew.h>


#ifndef PARTICLESIM_OPENGLBUFFER_H
#define PARTICLESIM_OPENGLBUFFER_H


namespace renderbox {

    class OpenGLBuffer {
        GLuint bufferID;
    public:
        OpenGLBuffer();
        GLuint getBufferID();
        void bindBuffer(GLenum target = GL_ARRAY_BUFFER);
        static void unbindBuffer(GLenum target = GL_ARRAY_BUFFER);
        void buffer(const void *data, GLsizei size, GLenum usage = GL_STATIC_DRAW);
    };

}


#endif //PARTICLESIM_OPENGLBUFFER_H

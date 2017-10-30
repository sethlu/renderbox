#ifndef VOX_OPENGLBUFFER_H
#define VOX_OPENGLBUFFER_H


#include "platform.h"
#if defined(__MACOSX__)
#include <OpenGL/gl3.h>
#elif defined(__IPHONEOS__)
#include <OpenGLES/ES3/gl.h>
#endif

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


#endif //VOX_OPENGLBUFFER_H

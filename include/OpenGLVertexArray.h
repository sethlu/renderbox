#ifndef VOX_OPENGLVERTEXARRAY_H
#define VOX_OPENGLVERTEXARRAY_H


#include "platform.h"
#if defined(__MACOSX__)
#include <OpenGL/OpenGL.h>
#elif defined(__IPHONEOS__)
#include <OpenGLES/ES3/gl.h>
#endif
#include "OpenGLBuffer.h"
#include "OpenGLProgram.h"

namespace renderbox {

    class OpenGLVertexArray {
        GLuint vertexArrayID;
    public:
        OpenGLVertexArray();
        GLuint getVertexArrayID();
        void bindVertexArray();
        static void unbindVertexArray();
        void enableAttribute(OpenGLProgram *program, const char *name);
        void setAttributeBuffer(GLuint location,
                                OpenGLBuffer *buffer,
                                GLint size = 3,
                                GLenum type = GL_FLOAT,
                                GLsizei stride = 0,
                                const GLvoid *pointer = (GLvoid *) 0);
        void setAttributeBuffer(OpenGLProgram *program,
                                const char *name,
                                OpenGLBuffer *buffer,
                                GLint size = 3,
                                GLenum type = GL_FLOAT,
                                GLsizei stride = 0,
                                const GLvoid *pointer = (GLvoid *) 0);
        void setElementBuffer(OpenGLBuffer *buffer);
    };

}


#endif //VOX_OPENGLVERTEXARRAY_H
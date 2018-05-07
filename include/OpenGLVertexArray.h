#ifndef RENDERBOX_OPENGLVERTEXARRAY_H
#define RENDERBOX_OPENGLVERTEXARRAY_H


#include "platform.h"
#if defined(__MACOSX__)
#include <OpenGL/gl3.h>
#elif defined(__IPHONEOS__)
#include <OpenGLES/ES3/gl.h>
#endif
#include "OpenGLBuffer.h"
#include "OpenGLProgram.h"

namespace renderbox {

    class OpenGLVertexArray {
    protected:

        GLuint vertexArrayId;

    public:

        OpenGLVertexArray();

        GLuint getVertexArrayId() const;

        void bindVertexArray();

        static void unbindVertexArray();

        void enableAttribute(OpenGLProgram *program, const char *name);

        void disableAttribute(OpenGLProgram *program, const char *name);

        void setAttributeBuffer(GLuint index,
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

        void setAttribute(GLuint index, const vec2 &value);

        void setElementBuffer(OpenGLBuffer *buffer);

    };

}


#endif //RENDERBOX_OPENGLVERTEXARRAY_H

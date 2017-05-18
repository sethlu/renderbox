#include <iostream>
#include "OpenGLBuffer.h"


namespace renderbox {

    OpenGLBuffer::OpenGLBuffer() {
        glGenBuffers(1, &bufferID);
    }

    GLuint OpenGLBuffer::getBufferID() {
        return bufferID;
    }

    void OpenGLBuffer::bindBuffer(GLenum target) {
        glBindBuffer(target, bufferID);
    }

    void OpenGLBuffer::unbindBuffer(GLenum target) {
        glBindBuffer(target, 0);
    }

    void OpenGLBuffer::buffer(const void *data, GLsizei size, GLenum usage) {

        // glNamedBufferData(bufferID, size, data, usage); // OpenGL 4.5+

        bindBuffer(GL_ARRAY_BUFFER);
        glBufferData(GL_ARRAY_BUFFER, size, data, usage);
        unbindBuffer();

    }

}
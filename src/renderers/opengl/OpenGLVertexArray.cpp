#include <iostream>
#include "OpenGLVertexArray.h"


namespace renderbox {

    OpenGLVertexArray::OpenGLVertexArray() {
        glGenVertexArrays(1, &vertexArrayId);
    }

    GLuint OpenGLVertexArray::getVertexArrayId() const {
        return vertexArrayId;
    }

    void OpenGLVertexArray::bindVertexArray() {
        glBindVertexArray(vertexArrayId);
    }

    void OpenGLVertexArray::unbindVertexArray() {
        glBindVertexArray(0);
    }

    void OpenGLVertexArray::enableAttribute(OpenGLProgram *program, const char *name) {
        bindVertexArray();
        glEnableVertexAttribArray((GLuint) program->getAttributeLocation(name));
        unbindVertexArray();
    }

    void OpenGLVertexArray::setAttributeBuffer(OpenGLProgram *program, const char *name, OpenGLBuffer *buffer,
                                               GLint size, GLenum type, GLsizei stride, const GLvoid *pointer) {
        setAttributeBuffer((GLuint) program->getAttributeLocation(name),
                           buffer, size, type, stride, pointer);
    }

    void OpenGLVertexArray::setAttributeBuffer(GLuint location, OpenGLBuffer *buffer, GLint size, GLenum type,
                                               GLsizei stride, const GLvoid *pointer) {
        bindVertexArray();
        buffer->bindBuffer(GL_ARRAY_BUFFER);
        glVertexAttribPointer(location, size, type, GL_FALSE, stride, pointer);
        unbindVertexArray();
        buffer->unbindBuffer();
    }

    void OpenGLVertexArray::setElementBuffer(OpenGLBuffer *buffer) {
        bindVertexArray();
        buffer->bindBuffer(GL_ELEMENT_ARRAY_BUFFER);
        unbindVertexArray();
        buffer->unbindBuffer();
    }

}
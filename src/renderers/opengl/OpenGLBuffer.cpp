#include <iostream>
#include "OpenGLBuffer.h"


namespace renderbox {

    OpenGLBuffer::OpenGLBuffer()
        : size(0) {
        glGenBuffers(1, &bufferId);
    }

    OpenGLBuffer::~OpenGLBuffer() {
        glDeleteBuffers(1, &bufferId);
    };

    void OpenGLBuffer::bindBuffer(GLenum target) {
        glBindBuffer(target, bufferId);
    }

    void OpenGLBuffer::unbindBuffer(GLenum target) {
        glBindBuffer(target, 0);
    }

    void OpenGLBuffer::buffer(const void *data, GLsizei size, GLenum usage) {
        bindBuffer(GL_ARRAY_BUFFER);
        glBufferData(GL_ARRAY_BUFFER, size, data, usage);
        unbindBuffer(GL_ARRAY_BUFFER);

        this->size = size; // Update size of data held in buffer
    }

    void OpenGLBuffer::buffer(const std::vector<vec3> &data, GLenum usage) {
        buffer(&data[0], (GLsizei) data.size() * sizeof(vec3), usage);
    };

    void OpenGLBuffer::buffer(const std::vector<uvec3> &data, GLenum usage) {
        buffer(&data[0], (GLsizei) data.size() * sizeof(uvec3), usage);
    }

    void OpenGLBuffer::buffer(const std::vector<vec2> &data, GLenum usage) {
        buffer(&data[0], (GLsizei) data.size() * sizeof(vec2), usage);
    }

}

#include <iostream>
#include "OpenGLBuffer.h"


namespace renderbox {

    OpenGLBuffer::OpenGLBuffer() {
        glGenBuffers(1, &bufferId);
    }

    GLuint OpenGLBuffer::getBufferId() const {
        return bufferId;
    }

    void OpenGLBuffer::bindBuffer(GLenum target) {
        glBindBuffer(target, bufferId);
    }

    void OpenGLBuffer::unbindBuffer(GLenum target) {
        glBindBuffer(target, 0);
    }

    void OpenGLBuffer::buffer(const void *data, GLsizei size, GLenum usage) {
        bindBuffer(GL_ARRAY_BUFFER);
        glBufferData(GL_ARRAY_BUFFER, size, data, usage);
        unbindBuffer();
    }

    void OpenGLBuffer::buffer(const std::vector<glm::vec3> &data, GLenum usage) {
        buffer(&data[0], (GLsizei) data.size() * sizeof(glm::vec3), usage);
    };

    void OpenGLBuffer::buffer(const std::vector<glm::uvec3> &data, GLenum usage) {
        buffer(&data[0], (GLsizei) data.size() * sizeof(glm::uvec3), usage);
    };

}

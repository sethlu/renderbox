#include <iostream>
#include "OpenGLObjectProperties.h"


namespace renderbox {

    OpenGLBuffer *OpenGLObjectProperties::getBuffer(int index) {
        auto result = buffers.find(index);
        if (result != buffers.end()) {
            return result->second;
        }
        auto *buffer = new OpenGLBuffer();
        buffers.insert(std::pair<int, OpenGLBuffer *>(index, buffer));
        return buffer;
    }

    OpenGLVertexArray *OpenGLObjectProperties::getVertexArray(int index) {
        auto result = vertexArrays.find(index);
        if (result != vertexArrays.end()) {
            return result->second;
        }
        auto *vertexArray = new OpenGLVertexArray();
        vertexArrays.insert(std::pair<int, OpenGLVertexArray *>(index, vertexArray));
        return vertexArray;
    }

}
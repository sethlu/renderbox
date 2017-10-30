#include <iostream>
#include "OpenGLObjectProperties.h"


namespace renderbox {

    OpenGLBuffer *OpenGLObjectProperties::getBuffer(int index) {
        std::unordered_map<int, OpenGLBuffer *>::iterator result = buffers.find(index);
        if (result != buffers.end()) {
            return result->second;
        }
        OpenGLBuffer *buffer = new OpenGLBuffer();
        buffers.insert(std::pair<int, OpenGLBuffer *>(index, buffer));
        return buffer;
    }

    OpenGLVertexArray *OpenGLObjectProperties::getVertexArray(int index) {
        std::unordered_map<int, OpenGLVertexArray *>::iterator result = vertexArrays.find(index);
        if (result != vertexArrays.end()) {
            return result->second;
        }
        OpenGLVertexArray *vertexArray = new OpenGLVertexArray();
        vertexArrays.insert(std::pair<int, OpenGLVertexArray *>(index, vertexArray));
        return vertexArray;
    }

}
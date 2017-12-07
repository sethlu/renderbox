#include <iostream>
#include "OpenGLObjectProperties.h"


namespace renderbox {

    OpenGLBuffer *OpenGLObjectProperties::getBuffer(int index) {

        auto result = buffers.find(index);
        if (result != buffers.end()) {
            return result->second.get();
        }

        auto buffer = new OpenGLBuffer();
        buffers.insert(std::pair<int, std::unique_ptr<OpenGLBuffer>>(index, std::unique_ptr<OpenGLBuffer>(buffer)));

        return buffer;

    }

    OpenGLVertexArray *OpenGLObjectProperties::getVertexArray(int index) {

        auto result = vertexArrays.find(index);
        if (result != vertexArrays.end()) {
            return result->second.get();
        }

        auto vertexArray = new OpenGLVertexArray();
        vertexArrays.insert(std::pair<int, std::unique_ptr<OpenGLVertexArray>>(index, std::unique_ptr<OpenGLVertexArray>(vertexArray)));

        return vertexArray;

    }

}
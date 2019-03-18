#include <iostream>
#include "OpenGLGeometryProperties.h"


namespace renderbox {

    OpenGLBuffer *OpenGLGeometryProperties::getBuffer(int index) {

        auto result = buffers.find(index);
        if (result != buffers.end()) {
            return result->second.get();
        }

        auto buffer = new OpenGLBuffer();
        buffers.insert(std::make_pair(index, std::unique_ptr<OpenGLBuffer>(buffer)));

        return buffer;

    }

    OpenGLVertexArray *OpenGLGeometryProperties::getVertexArray(int index) {

        auto result = vertexArrays.find(index);
        if (result != vertexArrays.end()) {
            return result->second.get();
        }

        auto vertexArray = new OpenGLVertexArray();
        vertexArrays.insert(std::make_pair(index, std::unique_ptr<OpenGLVertexArray>(vertexArray)));

        return vertexArray;

    }

}
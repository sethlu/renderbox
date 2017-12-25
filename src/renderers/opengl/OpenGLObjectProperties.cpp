#include <iostream>
#include "OpenGLObjectProperties.h"


namespace renderbox {

    OpenGLBuffer *OpenGLObjectProperties::getBuffer(int index) {

        auto result = buffers.find(index);
        if (result != buffers.end()) {
            return result->second.get();
        }

        auto buffer = new OpenGLBuffer();
        buffers.insert(std::make_pair(index, std::unique_ptr<OpenGLBuffer>(buffer)));

        return buffer;

    }

    OpenGLVertexArray *OpenGLObjectProperties::getVertexArray(int index) {

        auto result = vertexArrays.find(index);
        if (result != vertexArrays.end()) {
            return result->second.get();
        }

        auto vertexArray = new OpenGLVertexArray();
        vertexArrays.insert(std::make_pair(index, std::unique_ptr<OpenGLVertexArray>(vertexArray)));

        return vertexArray;

    }

    OpenGLTexture *OpenGLObjectProperties::getTexture(int index, bool *blankTexture) {

        auto result = textures.find(index);
        if (result != textures.end()) {
            if (blankTexture) *blankTexture = false;
            return result->second.get();
        }

        auto texture = new OpenGLTexture();
        textures.insert(std::make_pair(index, std::unique_ptr<OpenGLTexture>(texture)));

        if (blankTexture) *blankTexture = true;
        return texture;

    }

}
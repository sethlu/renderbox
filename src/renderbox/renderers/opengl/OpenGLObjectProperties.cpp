#include <iostream>
#include "OpenGLObjectProperties.h"


namespace renderbox {

    std::unordered_map<int, OpenGLProgram *> OpenGLObjectProperties::programs;

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

    OpenGLProgram *OpenGLObjectProperties::getProgram(Material *material) {
        std::unordered_map<int, OpenGLProgram *>::iterator result = programs.find(material->getMaterialID());
        if (result != programs.end()) {
            return result->second;
        }
        OpenGLProgram *program = new OpenGLProgram("shaders/debug_vert.glsl", "shaders/debug_frag.glsl"); // TODO
        programs.insert(std::pair<int, OpenGLProgram *>(material->getMaterialID(), program));
        return program;
    }

}
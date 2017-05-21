#include <iostream>
#include "OpenGLObjectProperties.h"
#include "../../materials/GLSLShaderMaterial.h"


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
        int materialID = material->getMaterialID();
        std::unordered_map<int, OpenGLProgram *>::iterator result = programs.find(materialID);
        if (result != programs.end()) {
            return result->second;
        }
        OpenGLProgram *program;
        switch (material->getMaterialType()) {
            case GLSL_SHADER_MATERIAL:
                program = new OpenGLProgram(((GLSLShaderMaterial *) material)->getVertexShaderSource(),
                                            ((GLSLShaderMaterial *) material)->getFragmentShaderSource());
                break;
            default:
                fprintf(stderr, "Unsupported material type");
                throw 2;
        }
        programs.insert(std::pair<int, OpenGLProgram *>(materialID, program));
        return program;
    }

}
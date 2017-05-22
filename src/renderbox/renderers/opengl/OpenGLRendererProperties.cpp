#include <iostream>
#include "OpenGLRendererProperties.h"
#include "../../materials/GLSLShaderMaterial.h"


namespace renderbox {

    std::unordered_map<int, OpenGLProgram *> OpenGLRendererProperties::programs;

    OpenGLObjectProperties *OpenGLRendererProperties::getObjectProperties(Object *object) {
        std::unordered_map<int, OpenGLObjectProperties *>::iterator result = objectProperties.find(object->getObjectID());
        if (result != objectProperties.end()) {
            return result->second;
        }
        OpenGLObjectProperties *properties = new OpenGLObjectProperties();
        objectProperties.insert(std::pair<int, OpenGLObjectProperties *>(object->getObjectID(), properties));
        return properties;
    }

    OpenGLProgram *OpenGLRendererProperties::getProgram(Material *material) {
        int materialID = material->getMaterialID();
        std::unordered_map<int, OpenGLProgram *>::iterator result = programs.find(materialID);
        if (result != programs.end()) {
            return result->second;
        }
        OpenGLProgram *program;
        switch (material->getMaterialType()) {
            case MESH_BASIC_MATERIAL:
                program = new OpenGLProgram(
                    #include "../shaders/glsl/lib/mesh_basic_vert.glsl"
                        ,
                    #include "../shaders/glsl/lib/mesh_basic_frag.glsl"
                );
                break;
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
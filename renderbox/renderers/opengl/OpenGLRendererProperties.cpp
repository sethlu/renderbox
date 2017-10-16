#include <iostream>
#include "OpenGLRendererProperties.h"
#include "../../materials/GLSLShaderMaterial.h"


namespace renderbox {

    std::unordered_map<int, OpenGLProgram *> OpenGLRendererProperties::programs;

    OpenGLObjectProperties *OpenGLRendererProperties::getObjectProperties(Object *object) {
        auto result = objectProperties.find(object->getObjectID());
        if (result != objectProperties.end()) {
            return result->second;
        }
        auto *properties = new OpenGLObjectProperties();
        objectProperties.insert(std::pair<int, OpenGLObjectProperties *>(object->getObjectID(), properties));
        return properties;
    }

    OpenGLProgram *OpenGLRendererProperties::getProgram(Material *material) {
        int materialID = material->getMaterialID();
        auto result = programs.find(materialID);
        if (result != programs.end()) {
            return result->second;
        }
        OpenGLProgram *program;
        switch (material->getMaterialType()) {
            case MESH_BASIC_MATERIAL:
                program = new OpenGLProgram(
                    #if defined(__MACOSX__)
                    #include "shaders/mesh_basic_vert.glsl"
                    #elif defined(__IPHONEOS__)
                    #include "shaders/es/mesh_basic_vert.glsl"
                    #endif
                    ,
                    #if defined(__MACOSX__)
                    #include "shaders/mesh_basic_frag.glsl"
                    #elif defined(__IPHONEOS__)
                    #include "shaders/es/mesh_basic_frag.glsl"
                    #endif
                );
                break;
            case MESH_LAMBERT_MATERIAL:
                program = new OpenGLProgram(
                    #if defined(__MACOSX__)
                    #include "shaders/mesh_lambert_vert.glsl"
                    #elif defined(__IPHONEOS__)
                    #include "shaders/es/mesh_lambert_vert.glsl"
                    #endif
                    ,
                    #if defined(__MACOSX__)
                    #include "shaders/mesh_lambert_frag.glsl"
                    #elif defined(__IPHONEOS__)
                    #include "shaders/es/mesh_lambert_frag.glsl"
                    #endif
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

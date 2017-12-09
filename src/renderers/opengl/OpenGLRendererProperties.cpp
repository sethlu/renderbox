#include <iostream>
#include "OpenGLRendererProperties.h"
#include "GLSLMaterial.h"


namespace renderbox {

    std::unordered_map<int, std::unique_ptr<OpenGLProgram>> OpenGLRendererProperties::programs;

    OpenGLObjectProperties *OpenGLRendererProperties::getObjectProperties(Object *object) {

        auto result = objectProperties.find(object->getObjectId());
        if (result != objectProperties.end()) {
            return result->second.get();
        }

        auto *properties = new OpenGLObjectProperties();
        objectProperties.insert(std::pair<int, std::unique_ptr<OpenGLObjectProperties>>(object->getObjectId(), std::unique_ptr<OpenGLObjectProperties>(properties)));

        return properties;

    }

    OpenGLProgram *OpenGLRendererProperties::getProgram(Material *material) {

        int materialId = material->getMaterialId();
        auto result = programs.find(materialId);
        if (result != programs.end()) {
            return result->second.get();
        }

        OpenGLProgram *program;
        switch (material->getMaterialType()) {
            case MESH_BASIC_MATERIAL:
                program = OpenGLProgram::createProgramWithPreprocessedSources(
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
                program = OpenGLProgram::createProgramWithPreprocessedSources(
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
            case GLSL_MATERIAL:
                program = new OpenGLProgram(((GLSLMaterial *) material)->getVertexShaderSource(),
                                            ((GLSLMaterial *) material)->getFragmentShaderSource());
                break;
            default:
                fprintf(stderr, "Unsupported material type");
                throw 2;
        }

        programs.insert(std::pair<int, std::unique_ptr<OpenGLProgram>>(materialId, std::unique_ptr<OpenGLProgram>(program)));

        return program;

    }

}

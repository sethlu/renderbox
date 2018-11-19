#include <sstream>
#include <iostream>
#include "OpenGLRendererProperties.h"
#include "GLSLMaterial.h"


namespace renderbox {

    std::unordered_map<int, std::unique_ptr<OpenGLProgram>> OpenGLRendererProperties::programs;

    OpenGLObjectProperties *OpenGLRendererProperties::getObjectProperties(Object *object, bool *blankObjectProperties) {

        auto result = objectProperties.find(object->getObjectId());
        if (result != objectProperties.end()) {
            if (blankObjectProperties) *blankObjectProperties = false;
            return result->second.get();
        }

        auto *properties = new OpenGLObjectProperties();
        objectProperties.insert(std::pair<int, std::unique_ptr<OpenGLObjectProperties>>(object->getObjectId(), std::unique_ptr<OpenGLObjectProperties>(properties)));

        if (blankObjectProperties) *blankObjectProperties = true;
        return properties;

    }

    unsigned long upperPowerOfTwo(unsigned long v) {
        v--;
        v |= v >> 1;
        v |= v >> 2;
        v |= v >> 4;
        v |= v >> 8;
        v |= v >> 16;
        v++;
        return v;
    }

    OpenGLProgram *OpenGLRendererProperties::getProgram(Material *material, bool forceRecompile) {

        std::unique_ptr<OpenGLProgram> *owner = nullptr;

        int materialId = material->getMaterialId();
        auto result = programs.find(materialId);
        if (result != programs.end()) {
            if (!forceRecompile) {
                return result->second.get();
            }
            owner = &result->second;
        }

        std::ostringstream bootstrap;

#define BOOTSTRAP(VAR, VAL) bootstrap << "#define " #VAR " " << (VAL) << "\n";

        BOOTSTRAP(RB_NUM_POINT_LIGHTS, upperPowerOfTwo(numPointLights));

        // TODO: Program needs to be invalidated if material updates
		if (material->isAmbientMaterial()) {
			if (auto m = dynamic_cast<AmbientMaterial *>(material)) {
				if (m->getAmbientMap()) {
					BOOTSTRAP(RB_MATERIAL_AMBIENT_MAP, ""); // Has ambient map
				}
			}
		}
        if (material->isDiffuseMaterial()) {
            if (auto m = dynamic_cast<DiffuseMaterial *>(material)) {
                if (m->getDiffuseMap()) {
                    BOOTSTRAP(RB_MATERIAL_DIFFUSE_MAP, ""); // Has diffuse map
                }
            }
        }

#undef BOOTSTRAP

        OpenGLProgram *program;
        switch (material->getMaterialType()) {
            case MESH_BASIC_MATERIAL:
                program = OpenGLProgram::createProgramWithPreprocessedSources(
                    #if defined(RENDERBOX_OS_MACOS)
                    #include "shaders/mesh_basic_vert.glsl"
                    #elif defined(RENDERBOX_OS_IPHONEOS)
                    #include "shaders/es/mesh_basic_vert.glsl"
                    #endif
                    ,
                    #if defined(RENDERBOX_OS_MACOS)
                    #include "shaders/mesh_basic_frag.glsl"
                    #elif defined(RENDERBOX_OS_IPHONEOS)
                    #include "shaders/es/mesh_basic_frag.glsl"
                    #endif
                    , bootstrap.str()
                );
                break;
            case MESH_LAMBERT_MATERIAL:
                program = OpenGLProgram::createProgramWithPreprocessedSources(
                    #if defined(RENDERBOX_OS_MACOS)
                    #include "shaders/mesh_lambert_vert.glsl"
                    #elif defined(RENDERBOX_OS_IPHONEOS)
                    #include "shaders/es/mesh_lambert_vert.glsl"
                    #endif
                    ,
                    #if defined(RENDERBOX_OS_MACOS)
                    #include "shaders/mesh_lambert_frag.glsl"
                    #elif defined(RENDERBOX_OS_IPHONEOS)
                    #include "shaders/es/mesh_lambert_frag.glsl"
                    #endif
                    , bootstrap.str()
                );
                break;
            case GLSL_MATERIAL:
                program = OpenGLProgram::createProgramWithPreprocessedSources(
                    ((GLSLMaterial *) material)->getVertexShaderSource(),
                    ((GLSLMaterial *) material)->getFragmentShaderSource()
                );
                break;
            default:
                fprintf(stderr, "Unsupported material type");
                throw 2;
        }

        if (owner) {
            (*owner).reset(program);
        } else {
            programs.insert(std::pair<int, std::unique_ptr<OpenGLProgram>>(
                materialId, std::unique_ptr<OpenGLProgram>(program)));
        }

        return program;

    }

}

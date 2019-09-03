#include "OpenGLRendererProperties.h"

#include <sstream>
#include <iostream>

#include "GLSLMaterial.h"
#include "logging.h"


namespace renderbox {

    std::unordered_map<Material *, ObjectOpenGLProgram> OpenGLRendererProperties::programs;

    OpenGLGeometryProperties *
    OpenGLRendererProperties::getGeometryProperties(Geometry const *geometry, bool *blankProperties) {

        auto result = geometryProperties.find(geometry);
        if (result != geometryProperties.end()) {
            if (blankProperties) *blankProperties = false;
            return result->second.get();
        }

        auto *properties = new OpenGLGeometryProperties();
        geometryProperties.insert(std::make_pair(geometry, std::unique_ptr<OpenGLGeometryProperties>(properties)));

        if (blankProperties) *blankProperties = true;
        return properties;

    }

    OpenGLMaterialProperties *
    OpenGLRendererProperties::getMaterialProperties(Material const *material, bool *blankProperties) {

        auto result = materialProperties.find(material);
        if (result != materialProperties.end()) {
            if (blankProperties) *blankProperties = false;
            return result->second.get();
        }

        auto *properties = new OpenGLMaterialProperties();
        materialProperties.insert(std::make_pair(material, std::unique_ptr<OpenGLMaterialProperties>(properties)));

        if (blankProperties) *blankProperties = true;
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
        auto const &openglMaterial = dynamic_cast<OpenGLMaterial *>(material);
        if (!openglMaterial) {
            LOG(ERROR) << "Unsupported material type" << std::endl;
            exit(EXIT_FAILURE);
        };

        ObjectOpenGLProgram *objectOpenGLProgram = nullptr;

        auto result = programs.find(material);
        if (result != programs.end()) {
            if (result->second.materialVersion == material->getVersion() && !forceRecompile) {
                return result->second.program;
            } else {
                LOG(VERBOSE) << "getProgram: MISS (invalidated)" << std::endl;
                objectOpenGLProgram = &result->second;
            }
        } else {
            LOG(VERBOSE) << "getProgram: MISS (cold)" << std::endl;
        }

        std::ostringstream bootstrap;

#define BOOTSTRAP(VAR, VAL) bootstrap << "#define " #VAR " " << (VAL) << "\n";

        BOOTSTRAP(RB_NUM_POINT_LIGHTS, upperPowerOfTwo(numPointLights));

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

        OpenGLProgram *program = OpenGLProgram::createProgramWithPreprocessedSources(
                openglMaterial->getOpenGLVertexShaderSource(nullptr),
                openglMaterial->getOpenGLFragmentShaderSource(nullptr),
                bootstrap.str());

        if (objectOpenGLProgram) {
            objectOpenGLProgram->program = program;
            objectOpenGLProgram->materialVersion = material->getVersion();
        } else {
            programs.insert(std::make_pair(material, (ObjectOpenGLProgram) {
                program,
                material->getVersion()
            }));
        }

        return program;

    }

}

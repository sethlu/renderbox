#include "OpenGLRendererProperties.h"

#include <sstream>
#include <iostream>

#include "GLSLMaterial.h"
#include "logging.h"


namespace renderbox {

    std::unordered_map<std::pair<Material::id_type , Geometry::id_type>, ObjectOpenGLProgram>
            OpenGLRendererProperties::_objectOpenGLPrograms;

    std::unordered_map<std::string, std::shared_ptr<OpenGLProgram>>
            OpenGLRendererProperties::_cachedOpenGLPrograms;

    OpenGLObjectProperties *OpenGLRendererProperties::getObjectProperties(Object const *object, bool *blankProperties) {

        auto objectId = object->getObjectId();

        auto result = objectProperties.find(objectId);
        if (result != objectProperties.end()) {
            if (blankProperties) *blankProperties = false;
            return result->second.get();
        }

        auto *properties = new OpenGLObjectProperties();
        objectProperties.insert(std::make_pair(objectId, std::unique_ptr<OpenGLObjectProperties>(properties)));

        if (blankProperties) *blankProperties = true;
        return properties;

    }

    OpenGLGeometryProperties *
    OpenGLRendererProperties::getGeometryProperties(Geometry const *geometry, bool *blankProperties) {

        auto geometryId = geometry->getGeometryId();

        auto result = geometryProperties.find(geometryId);
        if (result != geometryProperties.end()) {
            if (blankProperties) *blankProperties = false;
            return result->second.get();
        }

        auto *properties = new OpenGLGeometryProperties();
        geometryProperties.insert(std::make_pair(geometryId, std::unique_ptr<OpenGLGeometryProperties>(properties)));

        if (blankProperties) *blankProperties = true;
        return properties;

    }

    OpenGLMaterialProperties *
    OpenGLRendererProperties::getMaterialProperties(Material const *material, bool *blankProperties) {

        auto materialId = material->getMaterialId();

        auto result = materialProperties.find(materialId);
        if (result != materialProperties.end()) {
            if (blankProperties) *blankProperties = false;
            return result->second.get();
        }

        auto *properties = new OpenGLMaterialProperties();
        materialProperties.insert(std::make_pair(materialId, std::unique_ptr<OpenGLMaterialProperties>(properties)));

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

    OpenGLProgram *OpenGLRendererProperties::getProgram(
            Material const *material,
            Geometry const *geometry) {
        auto const &openglMaterial = dynamic_cast<OpenGLMaterial const *>(material);
        if (!openglMaterial) {
            LOG(ERROR) << "Unsupported material type" << std::endl;
            exit(EXIT_FAILURE);
        }

        auto const &materialGeometryPair = std::make_pair(material->getMaterialId(), geometry->getGeometryId());

        // There's one ObjectOpenGLProgram per object
        // It caches the pointer to the OpenGLProgram,
        // and records the version of object material and geometry
        ObjectOpenGLProgram *objectOpenGLProgram = nullptr;

        {
            auto result = _objectOpenGLPrograms.find(materialGeometryPair);
            if (result != _objectOpenGLPrograms.end()) {
                objectOpenGLProgram = &result->second;
                if (objectOpenGLProgram->materialVersion == material->getVersion() &&
                    objectOpenGLProgram->geometryVersion == geometry->getVersion() &&
                    objectOpenGLProgram->numPointLights == numPointLights) {
                    return result->second.program;
                } else {
                    LOG(VERBOSE) << "getProgram: LOCAL MISS (invalidated)" << std::endl;
                }
            } else {
                LOG(VERBOSE) << "getProgram: LOCAL MISS (cold)" << std::endl;
            }
        }

        // Object cache miss

        auto const &vertexShaderTag = openglMaterial->getOpenGLVertexShaderTag(geometry);
        auto const &fragmentShaderTag = openglMaterial->getOpenGLFragmentShaderTag(geometry);

        auto const openglProgramCacheTag =
                vertexShaderTag + "$" + fragmentShaderTag + "$pointlights-" + std::to_string(numPointLights);
        OpenGLProgram *openglProgram = nullptr;

        {
            auto result = _cachedOpenGLPrograms.find(openglProgramCacheTag);
            if (result != _cachedOpenGLPrograms.end()) {
                openglProgram = result->second.get();
            } else {
                LOG(VERBOSE) << "getProgram: GLOBAL MISS (cold)" << std::endl;

                std::ostringstream bootstrap;

#define BOOTSTRAP(VAR, VAL) bootstrap << "#define " #VAR " " << (VAL) << "\n";

                BOOTSTRAP(RB_NUM_POINT_LIGHTS, upperPowerOfTwo(numPointLights));

                if (material->isAmbientMaterial()) {
                    if (auto m = dynamic_cast<AmbientMaterial const *>(material)) {
                        if (m->getAmbientMap()) {
                            BOOTSTRAP(RB_MATERIAL_AMBIENT_MAP, ""); // Has ambient map
                        }
                    }
                }
                if (material->isDiffuseMaterial()) {
                    if (auto m = dynamic_cast<DiffuseMaterial const *>(material)) {
                        if (m->getDiffuseMap()) {
                            BOOTSTRAP(RB_MATERIAL_DIFFUSE_MAP, ""); // Has diffuse map
                        }
                    }
                }

#undef BOOTSTRAP

                openglProgram = OpenGLProgram::createProgramWithPreprocessedSources(
                        openglMaterial->getOpenGLVertexShaderSource(geometry),
                        openglMaterial->getOpenGLFragmentShaderSource(geometry),
                        bootstrap.str());

                // Update global cache
                _cachedOpenGLPrograms.insert(std::make_pair(openglProgramCacheTag, openglProgram));
            }
        }

        // Update object cache
        if (objectOpenGLProgram) {
            objectOpenGLProgram->program = openglProgram;
            objectOpenGLProgram->materialVersion = material->getVersion();
            objectOpenGLProgram->geometryVersion = geometry->getVersion();
            objectOpenGLProgram->numPointLights = numPointLights;
        } else {
            _objectOpenGLPrograms.insert(std::make_pair(materialGeometryPair, (ObjectOpenGLProgram) {
                openglProgram,
                material->getVersion(),
                geometry->getVersion(),
                numPointLights
            }));
        }

        return openglProgram;

    }

}

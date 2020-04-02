#ifndef RENDERBOX_OPENGLRENDERERPROPERTIES_H
#define RENDERBOX_OPENGLRENDERERPROPERTIES_H


#include <unordered_map>
#include <string>

#include "std.h"
#include "SkinnedObject.h"
#include "OpenGL.h"
#include "OpenGLGeometryProperties.h"
#include "OpenGLMaterialProperties.h"


namespace renderbox {

    struct ObjectOpenGLProgram {
        OpenGLProgram *program;
        VersionTrackedObject::version_type materialVersion;
        VersionTrackedObject::version_type geometryVersion;
        unsigned numPointLights;
    };

    class OpenGLRendererProperties {
    public:

        OpenGLProgram *getProgram(Material const *material, Geometry const *geometry);

        OpenGLGeometryProperties *getGeometryProperties(Geometry const *geometry, bool *blankProperties = nullptr);

        OpenGLMaterialProperties *getMaterialProperties(Material const *material, bool *blankProperties = nullptr);

        unsigned numPointLights = 0;

        unsigned lastNumPointLights = 0;

    private:

        static std::unordered_map<std::pair<Material const *, Geometry const *>, ObjectOpenGLProgram>
                _objectOpenGLPrograms;

        static std::unordered_map<std::string, std::shared_ptr<OpenGLProgram>> _cachedOpenGLPrograms;

        std::unordered_map<Geometry const *, std::unique_ptr<OpenGLGeometryProperties>> geometryProperties;

        std::unordered_map<Material const *, std::unique_ptr<OpenGLMaterialProperties>> materialProperties;

    };

}


#endif //RENDERBOX_OPENGLRENDERERPROPERTIES_H

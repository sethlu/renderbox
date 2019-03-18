#ifndef RENDERBOX_OPENGLRENDERERPROPERTIES_H
#define RENDERBOX_OPENGLRENDERERPROPERTIES_H


#include <unordered_map>
#include <string>

#include "SkinnedObject.h"
#include "OpenGL.h"
#include "OpenGLGeometryProperties.h"
#include "OpenGLMaterialProperties.h"


namespace renderbox {

    struct ObjectOpenGLProgram {
        OpenGLProgram *program;
        VersionTrackedObject::version_type materialVersion;
    };

    class OpenGLRendererProperties {
    public:

        OpenGLProgram *getProgram(Material *material, bool forceRecompile = false);

        OpenGLGeometryProperties *getGeometryProperties(Geometry const *geometry, bool *blankProperties = nullptr);

        OpenGLMaterialProperties *getMaterialProperties(Material const *material, bool *blankProperties = nullptr);

        unsigned numPointLights = 0;

        unsigned lastNumPointLights = 0;

    private:

        static std::unordered_map<Material *, ObjectOpenGLProgram> programs;

        std::unordered_map<Geometry const *, std::unique_ptr<OpenGLGeometryProperties>> geometryProperties;

        std::unordered_map<Material const *, std::unique_ptr<OpenGLMaterialProperties>> materialProperties;

    };

}


#endif //RENDERBOX_OPENGLRENDERERPROPERTIES_H

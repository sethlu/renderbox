#ifndef RENDERBOX_OPENGLRENDERERPROPERTIES_H
#define RENDERBOX_OPENGLRENDERERPROPERTIES_H


#include <unordered_map>
#include <string>

#include "std.h"
#include "SkinnedObject.h"
#include "OpenGL.h"
#include "OpenGLGeometryProperties.h"
#include "OpenGLMaterialProperties.h"
#include "OpenGLObjectProperties.h"


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

        OpenGLObjectProperties *getObjectProperties(Object const *object, bool *blankProperties = nullptr);

        OpenGLGeometryProperties *getGeometryProperties(Geometry const *geometry, bool *blankProperties = nullptr);

        OpenGLMaterialProperties *getMaterialProperties(Material const *material, bool *blankProperties = nullptr);

        unsigned numPointLights = 0;

    private:

        static std::unordered_map<std::pair<Material::id_type , Geometry::id_type>, ObjectOpenGLProgram>
                _objectOpenGLPrograms;

        static std::unordered_map<std::string, std::shared_ptr<OpenGLProgram>> _cachedOpenGLPrograms;

        std::unordered_map<Object::id_type, std::unique_ptr<OpenGLObjectProperties>> objectProperties;

        std::unordered_map<Geometry::id_type, std::unique_ptr<OpenGLGeometryProperties>> geometryProperties;

        std::unordered_map<Material::id_type, std::unique_ptr<OpenGLMaterialProperties>> materialProperties;

    };

}


#endif //RENDERBOX_OPENGLRENDERERPROPERTIES_H

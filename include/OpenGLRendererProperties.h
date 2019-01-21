#ifndef RENDERBOX_OPENGLRENDERERPROPERTIES_H
#define RENDERBOX_OPENGLRENDERERPROPERTIES_H


#include <unordered_map>
#include <string>

#include "SkinnedObject.h"
#include "OpenGL.h"
#include "OpenGLObjectProperties.h"

namespace renderbox {

    struct ObjectOpenGLProgram {
        OpenGLProgram *program;
        VersionTrackedObject::version_type materialVersion;
    };

    class OpenGLRendererProperties {
    public:

        OpenGLProgram *getProgram(Material *material, bool forceRecompile = false);

        OpenGLObjectProperties *getObjectProperties(Object *object, bool *blankObjectProperties = nullptr);

        unsigned numPointLights = 0;

        unsigned lastNumPointLights = 0;

    private:

        static std::unordered_map<Material *, ObjectOpenGLProgram> programs;

        std::unordered_map<int, std::unique_ptr<OpenGLObjectProperties>> objectProperties;

    };

}


#endif //RENDERBOX_OPENGLRENDERERPROPERTIES_H

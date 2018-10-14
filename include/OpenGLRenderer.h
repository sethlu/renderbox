#ifndef RENDERBOX_OPENGLRENDERER_H
#define RENDERBOX_OPENGLRENDERER_H


#include <map>

#include "OpenGL.h"
#include "Scene.h"
#include "Camera.h"
#include "Mesh.h"
#include "OpenGLRenderTarget.h"
#include "OpenGLRendererProperties.h"
#include "OpenGLRenderList.h"

namespace renderbox {

    class OpenGLRenderer {
    public:

        // Render

        void render(Scene *scene, Camera *camera, OpenGLRenderTarget *renderTarget, bool forceClear = false);

        void loadObject(Object *object);

    protected:

        OpenGLRendererProperties properties;

    };

}


#endif //RENDERBOX_OPENGLRENDERER_H

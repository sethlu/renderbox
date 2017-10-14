#ifndef VOX_OPENGLRENDERER_H
#define VOX_OPENGLRENDERER_H


#include <GL/glew.h>
#include <map>
#include "../../scenes/Scene.h"
#include "../../cameras/Camera.h"
#include "../../objects/Mesh.h"
#include "OpenGLRenderTarget.h"
#include "OpenGLRendererProperties.h"
#include "OpenGLRenderList.h"

namespace renderbox {

    class OpenGLRenderer {

    protected:

        static std::unordered_map<int, OpenGLProgram *> programs;

        OpenGLRendererProperties *properties = new OpenGLRendererProperties();

    public:

        void loadObject(Object *object);

        OpenGLRenderList * prepassRender(Scene *object, Camera *camera);
        void render(OpenGLRenderList *renderList, Scene *scene, Camera *camera, OpenGLRenderTarget *renderTarget, bool forceClear);
        void render(Scene *scene, Camera *camera, OpenGLRenderTarget *renderTarget = nullptr, bool forceClear = false);

        virtual int getFramebufferWidth();
        virtual int getFramebufferHeight();

    };

}


#endif //VOX_OPENGLRENDERER_H

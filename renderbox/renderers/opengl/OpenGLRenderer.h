#ifndef VOX_OPENGLRENDERER_H
#define VOX_OPENGLRENDERER_H


#include "../../platform.h"
#if defined(__MACOSX__)
#include <OpenGL/gl3.h>
#elif defined(__IPHONEOS__)
#include <OpenGLES/ES3/gl.h>
#endif
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
		
		GLint framebufferId = 0;
		
    public:

        void loadObject(Object *object);

        OpenGLRenderList * prepassRender(Scene *object, Camera *camera);
        void render(OpenGLRenderList *renderList, Scene *scene, Camera *camera, OpenGLRenderTarget *renderTarget, bool forceClear);
        void render(Scene *scene, Camera *camera, OpenGLRenderTarget *renderTarget = nullptr, bool forceClear = false);

        virtual int getFramebufferWidth() = 0;
        virtual int getFramebufferHeight() = 0;

    };

}


#endif //VOX_OPENGLRENDERER_H

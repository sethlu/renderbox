#ifndef VOX_OPENGLRENDERER_H
#define VOX_OPENGLRENDERER_H


#include "platform.h"
#if defined(__MACOSX__)
#include <OpenGL/OpenGL.h>
#elif defined(__IPHONEOS__)
#include <OpenGLES/ES3/gl.h>
#endif
#include <map>
#include "Scene.h"
#include "Camera.h"
#include "Mesh.h"
#include "OpenGLRenderTarget.h"
#include "OpenGLRendererProperties.h"
#include "OpenGLRenderList.h"

namespace renderbox {

	class OpenGLRenderer {
    protected:

        static std::unordered_map<int, OpenGLProgram *> programs;

        OpenGLRendererProperties *properties = new OpenGLRendererProperties();
		
		GLuint framebufferId = 0;
		
    public:

        void loadObject(Object *object);

		// Render

        OpenGLRenderList * prepassRender(Scene *object, Camera *camera);

		void render(OpenGLRenderList *renderList, Scene *scene, Camera *camera, OpenGLRenderTarget *renderTarget, bool forceClear);

		void render(Scene *scene, Camera *camera, OpenGLRenderTarget *renderTarget = nullptr, bool forceClear = false);

		// Window framebuffer

        virtual int getFramebufferWidth() const = 0;

        virtual int getFramebufferHeight() const = 0;

    };

}


#endif //VOX_OPENGLRENDERER_H

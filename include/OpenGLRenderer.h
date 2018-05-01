#ifndef VOX_OPENGLRENDERER_H
#define VOX_OPENGLRENDERER_H


#include "platform.h"
#if defined(__MACOSX__)
#include <OpenGL/gl3.h>
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
    public:

		// Render

		void render(Scene *scene, Camera *camera, OpenGLRenderTarget *renderTarget = nullptr, bool forceClear = false);

        void loadObject(Object *object);

		// Window framebuffer

        virtual int getFramebufferWidth() const = 0;

        virtual int getFramebufferHeight() const = 0;

    protected:

        OpenGLRendererProperties properties;

        GLuint framebufferId = 0;

    };

}


#endif //VOX_OPENGLRENDERER_H

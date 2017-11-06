#ifndef VOX_RENDERERTARGET_H
#define VOX_RENDERERTARGET_H


#include "platform.h"
#if defined(__MACOSX__)
#include <OpenGL/OpenGL.h>
#elif defined(__IPHONEOS__)
#include <OpenGLES/ES3/gl.h>
#endif

namespace renderbox {

    class OpenGLRenderTarget {
    protected:

        int framebufferWidth;

        int framebufferHeight;

        GLuint framebufferId;

        GLuint renderedTexturebufferId;

        GLuint renderedDepthbufferId;

        void setFramebufferSize(int width, int height);

    public:

        OpenGLRenderTarget(int width, int height);

        GLuint getFramebufferId();

        int getFramebufferWidth();

        int getFramebufferHeight();

    };

}


#endif //VOX_RENDERERTARGET_H

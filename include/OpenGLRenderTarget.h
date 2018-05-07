#ifndef RENDERBOX_RENDERERTARGET_H
#define RENDERBOX_RENDERERTARGET_H


#include "platform.h"
#if defined(__MACOSX__)
#include <OpenGL/gl3.h>
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


#endif //RENDERBOX_RENDERERTARGET_H

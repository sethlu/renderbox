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
        int width;
        int height;
        GLuint framebufferID;
        GLuint renderedTexturebufferID;
        GLuint renderedDepthbufferID;
        bool setSize(int width, int height);
    public:
        OpenGLRenderTarget(int width, int height);
        GLuint getFramebufferID();
        int getWidth();
        int getHeight();
    };

}


#endif //VOX_RENDERERTARGET_H

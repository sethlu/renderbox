#ifndef VOX_RENDERERTARGET_H
#define VOX_RENDERERTARGET_H


#include <GL/glew.h>

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

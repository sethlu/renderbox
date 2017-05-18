#include <GL/glew.h>


#ifndef PARTICLESIM_RENDERERTARGET_H
#define PARTICLESIM_RENDERERTARGET_H


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

#endif //PARTICLESIM_RENDERERTARGET_H

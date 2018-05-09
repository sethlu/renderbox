#ifndef RENDERBOX_OPENGLFRAMEBUFFER_H
#define RENDERBOX_OPENGLFRAMEBUFFER_H


#include "OpenGLRenderTarget.h"

namespace renderbox {

    class OpenGLFramebuffer : public OpenGLRenderTarget {
    public:

        OpenGLFramebuffer(int width, int height);

        GLuint getFramebufferId();

        int getFramebufferWidth() const override;

        int getFramebufferHeight() const override;

    protected:

        int framebufferWidth;

        int framebufferHeight;

        GLuint renderedTexturebufferId;

        GLuint renderedDepthbufferId;

        void setFramebufferSize(int width, int height);

    };

}


#endif //RENDERBOX_OPENGLFRAMEBUFFER_H

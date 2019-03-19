#ifndef RENDERBOX_OPENGLFRAMEBUFFER_H
#define RENDERBOX_OPENGLFRAMEBUFFER_H


#include "OpenGLRenderTarget.h"

namespace renderbox {

    class OpenGLFramebuffer : public OpenGLRenderTarget {
    public:

        OpenGLFramebuffer(unsigned int width, unsigned int height);

        unsigned int getFramebufferWidth() const override;

        unsigned int getFramebufferHeight() const override;

    protected:

        unsigned int framebufferWidth;

        unsigned int framebufferHeight;

        GLuint renderedTexturebufferId;

        GLuint renderedDepthbufferId;

        void setFramebufferSize(unsigned int width, unsigned int height);

    };

}


#endif //RENDERBOX_OPENGLFRAMEBUFFER_H

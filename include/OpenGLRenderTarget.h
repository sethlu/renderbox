#ifndef RENDERBOX_RENDERERTARGET_H
#define RENDERBOX_RENDERERTARGET_H


#include "OpenGL.h"

namespace renderbox {

    class OpenGLRenderTarget {
    public:

        GLuint getFramebufferId() const {
            return framebufferId;
        }

        virtual int getFramebufferWidth() const = 0;

        virtual int getFramebufferHeight() const = 0;

    protected:

        GLuint framebufferId = 0;

    };

}


#endif //RENDERBOX_RENDERERTARGET_H

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
    public:

        GLuint getFramebufferId() const {
            return framebufferId;
        }

        virtual int getFramebufferWidth() const = 0;

        virtual int getFramebufferHeight() const = 0;

    protected:

        GLuint framebufferId;

    };

}


#endif //RENDERBOX_RENDERERTARGET_H

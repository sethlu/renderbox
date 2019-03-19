#ifndef RENDERBOX_OPENGLGLFWRENDERER_H
#define RENDERBOX_OPENGLGLFWRENDERER_H


#include "platform.h"
#if defined(RENDERBOX_OS_MACOS)

#include "OpenGLRenderTarget.h"

#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>

namespace renderbox {

    class GLFWOpenGLRenderTarget : public OpenGLRenderTarget {
    public:

        GLFWOpenGLRenderTarget();

        ~GLFWOpenGLRenderTarget() override;

        GLFWwindow *getWindow() const;

        unsigned int getWindowWidth() const;

        unsigned int getWindowHeight() const;

        unsigned int getFramebufferWidth() const override;

        unsigned int getFramebufferHeight() const override;

        void frameDidRender() override;

    protected:

        GLFWwindow *window;

    };

}

#endif //defined(RENDERBOX_OS_MACOS)


#endif //RENDERBOX_OPENGLGLFWRENDERER_H

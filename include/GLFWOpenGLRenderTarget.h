#ifndef RENDERBOX_OPENGLGLFWRENDERER_H
#define RENDERBOX_OPENGLGLFWRENDERER_H


#include "platform.h"
#if defined(RENDERBOX_MACOS)

#include "OpenGLRenderTarget.h"

#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>

namespace renderbox {

    class GLFWOpenGLRenderTarget : public OpenGLRenderTarget {
    public:

        GLFWOpenGLRenderTarget();

        ~GLFWOpenGLRenderTarget();

        GLFWwindow *getWindow() const;

        int getWindowWidth() const;

        int getWindowHeight() const;

        int getFramebufferWidth() const override;

        int getFramebufferHeight() const override;

    protected:

        GLFWwindow *window;

    };

}

#endif //defined(RENDERBOX_MACOS)


#endif //RENDERBOX_OPENGLGLFWRENDERER_H

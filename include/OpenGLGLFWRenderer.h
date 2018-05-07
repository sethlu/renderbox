#ifndef RENDERBOX_OPENGLGLFWRENDERER_H
#define RENDERBOX_OPENGLGLFWRENDERER_H


#include "platform.h"
#if defined(__MACOSX__)

#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>
#include "OpenGLRenderer.h"

namespace renderbox {

    class OpenGLGLFWRenderer : public OpenGLRenderer {
    protected:

        GLFWwindow *window;

    public:

        OpenGLGLFWRenderer();

        ~OpenGLGLFWRenderer();

        GLFWwindow *getWindow() const;

        int getWindowWidth() const;

        int getWindowHeight() const;

        int getFramebufferWidth() const override;

        int getFramebufferHeight() const override;

    };

}

#endif // defined(__MACOSX__)


#endif //RENDERBOX_OPENGLGLFWRENDERER_H

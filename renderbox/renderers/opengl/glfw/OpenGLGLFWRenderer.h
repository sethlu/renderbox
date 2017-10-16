#ifndef VOX_OPENGLGLFWRENDERER_H
#define VOX_OPENGLGLFWRENDERER_H


#include "../../../platform.h"
#if defined(__MACOSX__)

#define GLFW_INCLUDE_GLCOREARB

#include "../OpenGLRenderer.h"
#include "GLFW/glfw3.h"

namespace renderbox {

    class OpenGLGLFWRenderer : public OpenGLRenderer {

    protected:

        GLFWwindow *window;

    public:

        OpenGLGLFWRenderer();
        ~OpenGLGLFWRenderer();

        GLFWwindow *getWindow();
        int getWindowWidth();
        int getWindowHeight();
        int getFramebufferWidth() override;
        int getFramebufferHeight() override;

    };

}

#endif // defined(__MACOSX__)

#endif //VOX_OPENGLGLFWRENDERER_H

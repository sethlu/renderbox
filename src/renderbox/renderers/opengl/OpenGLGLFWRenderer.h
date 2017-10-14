#ifndef VOX_OPENGLGLFWRENDERER_H
#define VOX_OPENGLGLFWRENDERER_H


#include "OpenGLRenderer.h"
#include "../../../../libs/glfw/include/GLFW/glfw3.h"

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
        int getFramebufferWidth();
        int getFramebufferHeight();

    };

}


#endif //VOX_OPENGLGLFWRENDERER_H

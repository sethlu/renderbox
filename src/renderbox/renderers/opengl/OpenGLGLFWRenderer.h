#ifndef PARTICLESIM_OPENGLGLFWRENDERER_H
#define PARTICLESIM_OPENGLGLFWRENDERER_H


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


#endif //PARTICLESIM_OPENGLGLFWRENDERER_H

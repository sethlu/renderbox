#include "../../../platform.h"
#if defined(__MACOSX__)

#include <OpenGL/gl3.h>
#include "OpenGLGLFWRenderer.h"


namespace renderbox {

    OpenGLGLFWRenderer::OpenGLGLFWRenderer() {

        // GLFW

        if (!glfwInit()) {
            fprintf(stderr, "Failed to initialize GLFW\n");
            throw 2;
        }

        glfwWindowHint(GLFW_SAMPLES, 4); // Anti-aliasing
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Create a window
        window = glfwCreateWindow(800, 600, "", nullptr, nullptr);
        if (!window) {

            fprintf(stderr, "Failed to open GLFW window\n");

            // Terminate GLFW
            glfwTerminate();
            throw 2;

        }
        glfwMakeContextCurrent(window);
        glfwSwapInterval(1);

        // Enable depth tests
        glEnable(GL_DEPTH_TEST);

    }

    OpenGLGLFWRenderer::~OpenGLGLFWRenderer() {
        glfwDestroyWindow(window);
    }

    GLFWwindow *OpenGLGLFWRenderer::getWindow() {
        return window;
    }

    int OpenGLGLFWRenderer::getWindowWidth() {
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        return width;
    }

    int OpenGLGLFWRenderer::getWindowHeight() {
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        return height;
    }

    int OpenGLGLFWRenderer::getFramebufferWidth() {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        return width;
    }

    int OpenGLGLFWRenderer::getFramebufferHeight() {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        return height;
    }

}

#endif // defined(__MACOSX__)
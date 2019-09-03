#include "GLFWOpenGLRenderTarget.h"

#include <cstdio>


namespace renderbox {

    GLFWOpenGLRenderTarget::GLFWOpenGLRenderTarget() {

        // GLFW

        char const *glfwError;

        if (!glfwInit()) {
            glfwGetError(&glfwError);
            fprintf(stderr, "Failed to initialize GLFW\n%s\n", glfwError);
            throw 2;
        }

        glfwWindowHint(GLFW_SAMPLES, 4); // Anti-aliasing
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

        // Create a window
        window = glfwCreateWindow(800, 600, "", nullptr, nullptr);
        if (!window) {
            glfwGetError(&glfwError);
            fprintf(stderr, "Failed to open GLFW window\n%s\n", glfwError);

            // Terminate GLFW
            glfwTerminate();
            throw 2;
        }
        glfwMakeContextCurrent(window);
        glfwSwapInterval(1);

    }

    GLFWOpenGLRenderTarget::~GLFWOpenGLRenderTarget() {
        glfwDestroyWindow(window);
    }

    GLFWwindow *GLFWOpenGLRenderTarget::getWindow() const {
        return window;
    }

    unsigned int GLFWOpenGLRenderTarget::getWindowWidth() const {
        int width;
        glfwGetWindowSize(window, &width, nullptr);
        return static_cast<unsigned int>(width);
    }

    unsigned int GLFWOpenGLRenderTarget::getWindowHeight() const {
        int height;
        glfwGetWindowSize(window, nullptr, &height);
        return static_cast<unsigned int>(height);
    }

    unsigned int GLFWOpenGLRenderTarget::getFramebufferWidth() const {
        int width;
        glfwGetFramebufferSize(window, &width, nullptr);
        return static_cast<unsigned int>(width);
    }

    unsigned int GLFWOpenGLRenderTarget::getFramebufferHeight() const {
        int height;
        glfwGetFramebufferSize(window, nullptr, &height);
        return static_cast<unsigned int>(height);
    }

    void GLFWOpenGLRenderTarget::frameDidRender() {
        OpenGLRenderTarget::frameDidRender();
    }

}

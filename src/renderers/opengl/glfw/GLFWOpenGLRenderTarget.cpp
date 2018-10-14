#include "platform.h"
#if defined(RENDERBOX_MACOS)

#include "GLFWOpenGLRenderTarget.h"

#include <cstdio>

namespace renderbox {

    GLFWOpenGLRenderTarget::GLFWOpenGLRenderTarget() {

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

    }

    GLFWOpenGLRenderTarget::~GLFWOpenGLRenderTarget() {
        glfwDestroyWindow(window);
    }

    GLFWwindow *GLFWOpenGLRenderTarget::getWindow() const {
        return window;
    }

    int GLFWOpenGLRenderTarget::getWindowWidth() const {
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        return width;
    }

    int GLFWOpenGLRenderTarget::getWindowHeight() const {
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        return height;
    }

    int GLFWOpenGLRenderTarget::getFramebufferWidth() const {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        return width;
    }

    int GLFWOpenGLRenderTarget::getFramebufferHeight() const {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        return height;
    }

}

#endif //defined(RENDERBOX_MACOS)

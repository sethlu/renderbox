#include "platform.h"
#if defined(RENDERBOX_OS_MACOS)

#include "GLFWOpenGLRenderTarget.h"

#include <cstdio>

#import <AppKit/AppKit.h>

#define GLFW_EXPOSE_NATIVE_NSGL
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <GLFWOpenGLRenderTarget.h>


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

    void GLFWOpenGLRenderTarget::frameDidRender() {
        OpenGLRenderTarget::frameDidRender();

        // The following is a rough fix to OpenGL drawing resulting in empty screen on macOS Mojave
        // Ref: https://github.com/glfw/glfw/issues/1334
        if (frameCount <= 2) [(NSOpenGLContext *) glfwGetNSGLContext(window) update];
    }

}

#endif //defined(RENDERBOX_OS_MACOS)

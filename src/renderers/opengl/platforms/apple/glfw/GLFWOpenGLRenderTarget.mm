#include "GLFWOpenGLRenderTarget.h"

#include <cstdio>

#import <AppKit/AppKit.h>

#define GLFW_EXPOSE_NATIVE_NSGL
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>


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

        // The following is a rough fix to OpenGL drawing resulting in empty screen on macOS Mojave
        // Ref: https://github.com/glfw/glfw/issues/1334
        if (frameCount <= 2) [(NSOpenGLContext *) glfwGetNSGLContext(window) update];
    }

}

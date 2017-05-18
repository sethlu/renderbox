#include <GL/glew.h>
#include "OpenGLRenderTarget.h"


namespace renderbox {

    bool OpenGLRenderTarget::setSize(int width, int height) {

        this->width = width;
        this->height = height;

        // Use frame buffer
        glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);

        // Texture buffer
        glTexImage2D(GL_TEXTURE_2D,
                     0, // Level
                     GL_RGB, // Internal format
                     this->width, // Width
                     this->height, // Height
                     0, // Border
                     GL_RGB, // Format
                     GL_UNSIGNED_BYTE, // Type
                     nullptr); // Data
        // Filtering
        // TODO: Why?
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        // Depth buffer
        glRenderbufferStorage(GL_RENDERBUFFER,
                              GL_DEPTH_COMPONENT,
                              this->width,
                              this->height);

        // Configure frame buffer
        // Texture buffer
        glFramebufferTexture(GL_FRAMEBUFFER, // Target
                             GL_COLOR_ATTACHMENT0, // Attachment
                             renderedTexturebufferID, // Texture
                             0); // Level

        // Specify a list of color buffers for drawing
        GLenum buffers[] = {GL_COLOR_ATTACHMENT0};
        glDrawBuffers(1, buffers);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            return false;
        }

        return true;

    }

    OpenGLRenderTarget::OpenGLRenderTarget(int width, int height) {
        // Generate frame buffer
        glGenFramebuffers(1, &framebufferID);
        // Create texture buffer
        glGenTextures(1, &renderedTexturebufferID);
        // Create depth buffer
        glGenRenderbuffers(1, &renderedDepthbufferID);

        // Use frame buffer
        glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);

        // Bind texture to frame buffer
        glBindTexture(GL_TEXTURE_2D, renderedTexturebufferID);
        // Bind depth buffer to frame buffer
        glBindRenderbuffer(GL_RENDERBUFFER, renderedDepthbufferID);

        // Attach render buffer object to the frame buffer object
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, // Target
                                  GL_DEPTH_ATTACHMENT, //  Attachment
                                  GL_RENDERBUFFER, // Render buffer target
                                  renderedDepthbufferID); // Render buffer

        setSize(width, height);
    }

    GLuint OpenGLRenderTarget::getFramebufferID() {
        return framebufferID;
    }

    int OpenGLRenderTarget::getWidth() {
        return width;
    }

    int OpenGLRenderTarget::getHeight() {
        return height;
    }

}
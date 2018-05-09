#include "platform.h"
#if defined(__MACOSX__)
#include <OpenGL/gl3.h>
#elif defined(__IPHONEOS__)
#include <OpenGLES/ES3/gl.h>
#endif
#include "OpenGLFramebuffer.h"


namespace renderbox {

    OpenGLFramebuffer::OpenGLFramebuffer(int width, int height) {
        // Generate frame buffer
        glGenFramebuffers(1, &framebufferId);
        // Create texture buffer
        glGenTextures(1, &renderedTexturebufferId);
        // Create depth buffer
        glGenRenderbuffers(1, &renderedDepthbufferId);

        // Use frame buffer
        glBindFramebuffer(GL_FRAMEBUFFER, framebufferId);

        // Bind texture to frame buffer
        glBindTexture(GL_TEXTURE_2D, renderedTexturebufferId);
        // Bind depth buffer to frame buffer
        glBindRenderbuffer(GL_RENDERBUFFER, renderedDepthbufferId);

        // Attach render buffer object to the frame buffer object
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, // Target
                                  GL_DEPTH_ATTACHMENT, //  Attachment
                                  GL_RENDERBUFFER, // Render buffer target
                                  renderedDepthbufferId); // Render buffer

        setFramebufferSize(width, height);
    }

    GLuint OpenGLFramebuffer::getFramebufferId() {
        return framebufferId;
    }

    int OpenGLFramebuffer::getFramebufferWidth() const {
        return framebufferWidth;
    }

    int OpenGLFramebuffer::getFramebufferHeight() const {
        return framebufferHeight;
    }

    void OpenGLFramebuffer::setFramebufferSize(int width, int height) {

        this->framebufferWidth = width;
        this->framebufferHeight = height;

        // Use frame buffer
        glBindFramebuffer(GL_FRAMEBUFFER, framebufferId);

        // Texture buffer
        glTexImage2D(GL_TEXTURE_2D,
                     0, // Level
                     GL_RGB, // Internal format
                     this->framebufferWidth, // Width
                     this->framebufferHeight, // Height
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
                              this->framebufferWidth,
                              this->framebufferHeight);

        // Configure frame buffer
        // Texture buffer
        glFramebufferTexture2D(GL_FRAMEBUFFER, // Target
                               GL_COLOR_ATTACHMENT0, // Attachment
                               GL_TEXTURE_2D,
                               renderedTexturebufferId, // Texture
                               0); // Level

        // Specify a list of color buffers for drawing
        GLenum buffers[] = {GL_COLOR_ATTACHMENT0};
        glDrawBuffers(1, buffers);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            throw 2;
        }

    }

}

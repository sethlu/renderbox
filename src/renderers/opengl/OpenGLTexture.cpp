#include "OpenGLTexture.h"


namespace renderbox {

    OpenGLTexture::OpenGLTexture() {
        glGenTextures(1, &textureId);
    }

    OpenGLTexture::~OpenGLTexture() {
        glDeleteTextures(1, &textureId);
    }

    void OpenGLTexture::bindTexture(GLenum target) {
        glBindTexture(target, textureId);
    }

    void OpenGLTexture::unbindTexture(GLenum target) {
        glBindTexture(target, 0);
    }

    void OpenGLTexture::texture(const void *pixels, unsigned width, unsigned height) {
        bindTexture(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
        unbindTexture(GL_TEXTURE_2D);
    }

    void OpenGLTexture::texture(std::shared_ptr<Texture> texture_) {
        // Expect first pixel to be at the lower left corner
        auto convertedTexture = Texture::convert(std::move(texture_),
                TEXTURE_PIXEL_FORMAT_RGB_UNSIGNED_BYTE, TEXTURE_COORDINATES_UR);

        texture(convertedTexture->pixels.get(),
                convertedTexture->width,
                convertedTexture->height);
    }

}

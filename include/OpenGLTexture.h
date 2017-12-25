#ifndef RENDERBOX_OPENGLTEXTURE_H
#define RENDERBOX_OPENGLTEXTURE_H


#include "platform.h"
#if defined(__MACOSX__)
#include <OpenGL/gl3.h>
#elif defined(__IPHONEOS__)
#include <OpenGLES/ES3/gl.h>
#endif
#include "Texture.h"

namespace renderbox {

    class OpenGLTexture {
    public:

        OpenGLTexture();

        virtual ~OpenGLTexture();

        GLuint getTextureId() const {
            return textureId;
        }

        void bindTexture(GLenum target = GL_TEXTURE_2D);

        static void unbindTexture(GLenum target = GL_TEXTURE_2D);

        void texture(const void *pixels, unsigned width, unsigned height);

        void texture(Texture *texture);

    protected:

        GLuint textureId;

    };

}


#endif //RENDERBOX_OPENGLTEXTURE_H

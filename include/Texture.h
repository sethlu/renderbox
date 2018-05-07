#ifndef RENDERBOX_TEXTURE_H
#define RENDERBOX_TEXTURE_H


#include <memory>

namespace renderbox {

    class Texture {

        friend class OpenGLTexture;

    public:

        explicit Texture(const char *pixels, unsigned width, unsigned height)
                : pixels(pixels), width(width), height(height) {};

        static Texture *fromFile(const char *filename);

        static Texture *fromBMPFile(const char *filename);

    protected:

        std::unique_ptr<const char> pixels;

        unsigned width;

        unsigned height;

    };

}


#endif //RENDERBOX_TEXTURE_H

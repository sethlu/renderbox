#ifndef RENDERBOX_TEXTURE_H
#define RENDERBOX_TEXTURE_H


#include <memory>

namespace renderbox {

    typedef enum {
                                                // [0]      [1]      [2]
        TEXTURE_PIXEL_FORMAT_RGB_UNSIGNED_BYTE, // RRRRRRRR GGGGGGGG BBBBBBBB
        TEXTURE_PIXEL_FORMAT_BGR_UNSIGNED_BYTE, // BBBBBBBB GGGGGGGG RRRRRRRR
    } TEXTURE_PIXEL_FORMAT;

    typedef enum {
        TEXTURE_COORDINATES_UR,
        TEXTURE_COORDINATES_DR,
    } TEXTURE_COORDINATES;

    class Texture {

        friend class OpenGLTexture;

    public:

        Texture(char const *pixels, unsigned size, unsigned width, unsigned height,
                TEXTURE_PIXEL_FORMAT pixelFormat, TEXTURE_COORDINATES coordinates);

        Texture(const Texture& texture);

        static std::shared_ptr<Texture> fromFile(const char *filename);

        static std::shared_ptr<Texture> fromFile(const std::string &filename);

        static std::shared_ptr<Texture> fromBMPFile(const char *filename);

        static std::shared_ptr<Texture> convert(
                std::shared_ptr<Texture> texture,
                TEXTURE_PIXEL_FORMAT newPixelFormat,
                TEXTURE_COORDINATES newCoordinates);

    protected:

        std::unique_ptr<char> pixels;

        unsigned size;

        unsigned width;

        unsigned height;

        TEXTURE_PIXEL_FORMAT pixelFormat;

        TEXTURE_COORDINATES coordinates;

    private:

        Texture(char *pixels, unsigned size, unsigned width, unsigned height,
                TEXTURE_PIXEL_FORMAT pixelFormat, TEXTURE_COORDINATES coordinates);

    };

}


#endif //RENDERBOX_TEXTURE_H

#ifndef RENDERBOX_TEXTURE_H
#define RENDERBOX_TEXTURE_H


#include <memory>

namespace renderbox {

    typedef enum {                                          // [0]      [1]      [2]      [3]
        TEXTURE_PIXEL_FORMAT_RGB_UNSIGNED_BYTE  = 0x0000,   // RRRRRRRR GGGGGGGG BBBBBBBB
        TEXTURE_PIXEL_FORMAT_RBG_UNSIGNED_BYTE  = 0x0010,   //
        TEXTURE_PIXEL_FORMAT_GRB_UNSIGNED_BYTE  = 0x0100,   //
        TEXTURE_PIXEL_FORMAT_GBR_UNSIGNED_BYTE  = 0x0110,   //
        TEXTURE_PIXEL_FORMAT_BRG_UNSIGNED_BYTE  = 0x1000,   //
        TEXTURE_PIXEL_FORMAT_BGR_UNSIGNED_BYTE  = 0x1010,   // BBBBBBBB GGGGGGGG RRRRRRRR
        TEXTURE_PIXEL_FORMAT_RGBA_UNSIGNED_BYTE = 0x0001,   // RRRRRRRR GGGGGGGG BBBBBBBB AAAAAAAA
        TEXTURE_PIXEL_FORMAT_BGRA_UNSIGNED_BYTE = 0x1011,   // BBBBBBBB GGGGGGGG RRRRRRRR AAAAAAAA
    } TEXTURE_PIXEL_FORMAT;

    inline unsigned texturePixelFormatSize(TEXTURE_PIXEL_FORMAT pixelFormat) {
        return (pixelFormat & 1) + 3;
    }

    typedef enum {
        TEXTURE_COORDINATES_UR,
        TEXTURE_COORDINATES_DR,
    } TEXTURE_COORDINATES;

    class Texture {

        friend class OpenGLTexture;
        friend class MetalTexture;

    public:

        Texture(unsigned char const *pixels, unsigned size, unsigned width, unsigned height,
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

        std::unique_ptr<unsigned char> pixels;

        unsigned size;

        unsigned width;

        unsigned height;

        TEXTURE_PIXEL_FORMAT pixelFormat;

        TEXTURE_COORDINATES coordinates;

    private:

        Texture(unsigned char *pixels, unsigned size, unsigned width, unsigned height,
                TEXTURE_PIXEL_FORMAT pixelFormat, TEXTURE_COORDINATES coordinates);

    };

}


#endif //RENDERBOX_TEXTURE_H

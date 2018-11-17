#include <ios>
#include <fstream>
#include "Texture.h"
#include "bitmap.h"


namespace renderbox {

    Texture::Texture(const char *pixels, unsigned size, unsigned width, unsigned height,
                     TEXTURE_PIXEL_FORMAT pixelFormat,
                     TEXTURE_COORDINATES coordinates)
            : size(size), width(width), height(height), pixelFormat(pixelFormat), coordinates(coordinates) {
        auto pixels_ = new char[size];
        memcpy(pixels_, pixels, size);
        this->pixels.reset(pixels_);
    }

    Texture::Texture(char *pixels, unsigned size, unsigned width, unsigned height,
                     TEXTURE_PIXEL_FORMAT pixelFormat,
                     TEXTURE_COORDINATES coordinates)
            : pixels(pixels), size(size), width(width), height(height), pixelFormat(pixelFormat),
              coordinates(coordinates) {

    }

    Texture::Texture(const Texture &texture)
            : size(texture.size), width(texture.width), height(texture.height), pixelFormat(texture.pixelFormat),
              coordinates(texture.coordinates) {
        auto pixels_ = new char[texture.size];
        memcpy(pixels_, texture.pixels.get(), texture.size);
        pixels.reset(pixels_);
    }

    std::shared_ptr<Texture> Texture::fromFile(const char *filename) {
        return fromBMPFile(filename); // TODO: Check file extension
    }

    std::shared_ptr<Texture> Texture::fromFile(const std::string &filename) {
        return fromFile(filename.c_str());
    }

    std::shared_ptr<Texture> Texture::fromBMPFile(const char *filename) {

        // Expect BMP pixel format to be TEXTURE_PIXEL_FORMAT_BGR_UNSIGNED_BYTE

        std::ifstream file(filename, std::ios::binary);

        if (!file) {
            // Fail to open file
            throw;
        }

        BITMAPFILEHEADER header{};
        BITMAPINFOHEADER info{};

        file.read(reinterpret_cast<char *>(&header), sizeof(BITMAPFILEHEADER));
        file.read(reinterpret_cast<char *>(&info), sizeof(BITMAPINFOHEADER));

        if (header.bfType != 'MB') {
            // Not a bitmap file
            throw 2;
        }

        bool flipVerical = false;
        if (info.biHeight < 0) {
            flipVerical = true;
            info.biHeight = -info.biHeight;
        }

        if (!info.biSizeImage) {
            info.biSizeImage = static_cast<unsigned>(info.biWidth * info.biHeight * 3);
        }

        auto *pixels = new char[info.biSizeImage];
        file.read(pixels, info.biSizeImage);

        return std::shared_ptr<Texture>(new Texture(
                pixels, info.biSizeImage,
                static_cast<unsigned>(info.biWidth), static_cast<unsigned>(info.biHeight),
                TEXTURE_PIXEL_FORMAT_BGR_UNSIGNED_BYTE,
                flipVerical ? TEXTURE_COORDINATES_DR : TEXTURE_COORDINATES_UR));

    }

    std::shared_ptr<Texture> Texture::convert(
            std::shared_ptr<Texture> texture,
            TEXTURE_PIXEL_FORMAT newPixelFormat,
            TEXTURE_COORDINATES newCoordinates) {
        if (newPixelFormat == texture->pixelFormat && newCoordinates == texture->coordinates) return texture;

        auto newTexture = std::make_shared<Texture>(*texture);

        if (newTexture->pixelFormat == newPixelFormat) {
            // No conversion necessary
        } else if ((newTexture->pixelFormat == TEXTURE_PIXEL_FORMAT_RGB_UNSIGNED_BYTE &&
             newPixelFormat == TEXTURE_PIXEL_FORMAT_BGR_UNSIGNED_BYTE) ||
            (newTexture->pixelFormat == TEXTURE_PIXEL_FORMAT_BGR_UNSIGNED_BYTE &&
             newPixelFormat == TEXTURE_PIXEL_FORMAT_RGB_UNSIGNED_BYTE)) {

            auto pixels = static_cast<char *>(newTexture->pixels.get());

            char t;
            for (auto i = 0; i < newTexture->size; i += 3) {
                t = pixels[i];
                pixels[i] = pixels[i + 2];
                pixels[i + 2] = t;
            }

        } else {
            fprintf(stderr, "Pixel format conversion not supported");
            throw;
        }

        if (newTexture->coordinates != newCoordinates) {
            unsigned pixelSize = 0; // Bytes
            switch (newTexture->pixelFormat) {
                default:
                    fprintf(stderr, "Coordinates conversion not supported for the given pixel format");
                    break;
                case TEXTURE_PIXEL_FORMAT_RGB_UNSIGNED_BYTE:
                case TEXTURE_PIXEL_FORMAT_BGR_UNSIGNED_BYTE:
                    pixelSize = 3;
                    break;
            }

            if ((newTexture->coordinates == TEXTURE_COORDINATES_DR && newCoordinates == TEXTURE_COORDINATES_UR) ||
                (newTexture->coordinates == TEXTURE_COORDINATES_UR && newCoordinates == TEXTURE_COORDINATES_DR)) {

                auto pixels = static_cast<char *>(newTexture->pixels.get());

                size_t rowSize = newTexture->width * pixelSize;
                char t[rowSize];
                for (int i = 0, h = newTexture->height / 2; i < h; ++i) {
                    char *a = pixels + i * rowSize,
                            *b = pixels + (newTexture->height - 1 - i) * rowSize;
                    memcpy(t, a, rowSize);
                    memcpy(a, b, rowSize);
                    memcpy(b, t, rowSize);
                }

            } else {
                fprintf(stderr, "Coordinates conversion not supported");
                throw;
            }
        }

        return newTexture;
    }

}

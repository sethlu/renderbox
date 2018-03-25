#include <ios>
#include <fstream>
#include "Texture.h"
#include "bitmap.h"


namespace renderbox {

    Texture *Texture::fromFile(const char *filename) {
        return fromBMPFile(filename); // TODO: Check file extension
    }

    Texture *Texture::fromBMPFile(const char *filename) {

        std::ifstream file(filename, std::ios::binary);

        if (!file) {
            // Fail to open file
            throw 2;
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
            info.biSizeImage = info.biWidth * info.biHeight * 3;
        }

        auto *pixels = new char[info.biSizeImage];
        file.read(pixels, info.biSizeImage);

        char t;
        for (auto i = 0; i < info.biSizeImage; i += 3) {
            t = pixels[i];
            pixels[i] = pixels[i + 2];
            pixels[i + 2] = t;
        }

        if (flipVerical) {
            size_t rowSize = info.biWidth * 3;
            char t[rowSize];
            for (int i = 0, h = info.biHeight / 2; i < h; ++i) {
                char *a = pixels + i * rowSize,
                     *b = pixels + (info.biHeight - 1 - i) * rowSize;
                memcpy(t, a, rowSize);
                memcpy(a, b, rowSize);
                memcpy(b, t, rowSize);
            }
        }

        return new Texture(pixels, static_cast<unsigned>(info.biWidth), static_cast<unsigned>(info.biHeight));

    }

}

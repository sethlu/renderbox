#include "MetalTexture.h"

namespace renderbox {

    MetalTexture::MetalTexture(id <MTLDevice> device) : device(device) {

    }

    void MetalTexture::texture(const void *pixels, NSUInteger width, NSUInteger height) {
        MTLTextureDescriptor *textureDescriptor =
            [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatBGRA8Unorm
                                                               width:width
                                                              height:height
                                                       mipmapped:NO];
        textureObject = [device newTextureWithDescriptor:textureDescriptor];
        MTLRegion region = {
            {0, 0, 0},          // MTLOrigin
            {width, height, 1}, // MTLSize
        };
        [textureObject replaceRegion:region
                         mipmapLevel:0
                           withBytes:pixels
                         bytesPerRow:4 * width];
    }

    void MetalTexture::texture(std::shared_ptr<Texture> texture_) {
        // Expect first pixel to be at the lower left corner
        auto convertedTexture = Texture::convert(std::move(texture_),
                                                 TEXTURE_PIXEL_FORMAT_BGRA_UNSIGNED_BYTE,
                                                 TEXTURE_COORDINATES_UR);

        texture(convertedTexture->pixels.get(),
                convertedTexture->width,
                convertedTexture->height);
    }

}

#ifndef RENDERBOX_METALTEXTURE_H
#define RENDERBOX_METALTEXTURE_H


#import <memory>

#import <Metal/Metal.h>

#import "Texture.h"

namespace renderbox {

    class MetalTexture {

        friend class MetalRenderer;

    public:

        explicit MetalTexture(id <MTLDevice> device);

        void texture(const void *pixels, NSUInteger width, NSUInteger height);

        void texture(std::shared_ptr<Texture> texture);

    protected:

        id <MTLDevice> device;

        id <MTLTexture> textureObject;

    };

}


#endif //RENDERBOX_METALTEXTURE_H

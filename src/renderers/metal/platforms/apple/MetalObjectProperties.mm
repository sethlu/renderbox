#include "MetalObjectProperties.h"


namespace renderbox {

    MetalObjectProperties::MetalObjectProperties(id <MTLDevice> device)
            : device(device) {

    }

    MetalBuffer *MetalObjectProperties::getBuffer(int index) {

        auto result = buffers.find(index);
        if (result != buffers.end()) {
            return result->second.get();
        }

        auto buffer = new MetalBuffer(device);
        buffers.insert(std::make_pair(index, std::unique_ptr<MetalBuffer>(buffer)));

        return buffer;

    }

    MetalTexture *MetalObjectProperties::getTexture(int index, bool *blankTexture) {

        auto result = textures.find(index);
        if (result != textures.end()) {
            if (blankTexture) *blankTexture = false;
            return result->second.get();
        }

        auto texture = new MetalTexture(device);
        textures.insert(std::make_pair(index, std::unique_ptr<MetalTexture>(texture)));

        if (blankTexture) *blankTexture = true;
        return texture;

    }

}

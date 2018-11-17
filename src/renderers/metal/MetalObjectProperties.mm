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

}

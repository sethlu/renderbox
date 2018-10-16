#include "MetalBuffer.h"


namespace renderbox {

    MetalBuffer::MetalBuffer(id<MTLDevice> device)
    : device(device) {
        
    }

    void MetalBuffer::buffer(const void *data, NSUInteger size, MTLResourceOptions options) {
        bufferObject = [device newBufferWithBytes:data length:size options:options];
        this->size = size;
    }
    
}

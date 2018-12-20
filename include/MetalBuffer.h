#ifndef RENDERBOX_METALBUFFER_H
#define RENDERBOX_METALBUFFER_H


#include <vector>

#import <Metal/Metal.h>

namespace renderbox {

    class MetalBuffer {

        friend class MetalRenderer;

    public:

        explicit MetalBuffer(id <MTLDevice> device);

        void buffer(const void *data, NSUInteger size, MTLResourceOptions options);

        template<typename T>
        void buffer(const std::vector<T> &data, MTLResourceOptions options = MTLResourceOptionCPUCacheModeDefault) {
            buffer(&data[0], data.size() * sizeof(T), options);
        }

        NSUInteger getSize() { return size; }

    protected:

        id <MTLDevice> device;

        id <MTLBuffer> bufferObject;

        NSUInteger size;

    };

}


#endif //RENDERBOX_METALBUFFER_H

#include "MetalDeviceRendererProperties.h"


namespace renderbox {

    MetalDeviceRendererProperties::MetalDeviceRendererProperties(id<MTLDevice> device)
            : device(device) {

        NSError *err;

        NSString *libPath = [[NSBundle mainBundle] pathForResource:@"RenderBoxMetalLibrary" ofType:@"metallib"];
        id<MTLLibrary> library = [device newLibraryWithFile:libPath error:&err];
        assert(err == nil);

        id<MTLFunction> fragmentFunction = [library newFunctionWithName:@"fragment_function"];
        id<MTLFunction> vertexFunction = [library newFunctionWithName:@"vertex_function"];

        renderPipelineDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
        [renderPipelineDescriptor setVertexFunction:vertexFunction];
        [renderPipelineDescriptor setFragmentFunction:fragmentFunction];
        renderPipelineDescriptor.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;

    }

    MetalObjectProperties *
    MetalDeviceRendererProperties::getObjectProperties(Object *object, bool *blankObjectProperties) {

        auto result = objectProperties.find(object->getObjectId());
        if (result != objectProperties.end()) {
            if (blankObjectProperties) *blankObjectProperties = false;
            return result->second.get();
        }

        auto *properties = new MetalObjectProperties(device);
        objectProperties.insert(
                std::make_pair(object->getObjectId(), std::unique_ptr<MetalObjectProperties>(properties)));

        if (blankObjectProperties) *blankObjectProperties = true;
        return properties;

    }

}

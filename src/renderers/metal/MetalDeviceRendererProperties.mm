#include "MetalDeviceRendererProperties.h"


namespace renderbox {

    MetalDeviceRendererProperties::MetalDeviceRendererProperties(id <MTLDevice> device)
            : device(device) {}

    MetalRenderPipelineState *
    MetalDeviceRendererProperties::getRenderPipelineState(Material *material) {

        auto result = renderPipelineStates.find(material->getMaterialId());
        if (result != renderPipelineStates.end()) {
            return result->second.get();
        }

        // Make new render pipeline descriptor

        id <MTLRenderPipelineState> renderPipelineStateObject;

        switch (material->getMaterialType()) {
            case MESH_LAMBERT_MATERIAL: {

                NSError *err;

                NSString *libPath = [[NSBundle mainBundle] pathForResource:@"RenderBoxMetalLibrary" ofType:@"metallib"];
                id <MTLLibrary> library = [device newLibraryWithFile:libPath error:&err];
                assert(err == nil);

                MTLRenderPipelineDescriptor *renderPipelineDescriptor = [[MTLRenderPipelineDescriptor alloc] init];

                id <MTLFunction> vertexFunction = [library newFunctionWithName:@"lambert_vert"];
                id <MTLFunction> fragmentFunction = [library newFunctionWithName:@"lambert_frag"];

                [renderPipelineDescriptor setVertexFunction:vertexFunction];
                [renderPipelineDescriptor setFragmentFunction:fragmentFunction];

                renderPipelineDescriptor.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;
                renderPipelineDescriptor.depthAttachmentPixelFormat = MTLPixelFormatDepth32Float;

                renderPipelineStateObject = [device newRenderPipelineStateWithDescriptor:renderPipelineDescriptor
                                                                                   error:&err];
                assert(err == nil);

                break;
            }
            default:
                fprintf(stderr, "Unsupported material type");
                throw;
        }

        // Save & return new render pipeline state

        MetalRenderPipelineState *renderPipelineState = new MetalRenderPipelineState(renderPipelineStateObject);
        renderPipelineStates.insert(std::make_pair(material->getMaterialId(),
                                                   std::unique_ptr<MetalRenderPipelineState>(renderPipelineState)));

        return renderPipelineState;

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

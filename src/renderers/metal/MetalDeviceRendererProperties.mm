#include "MetalDeviceRendererProperties.h"

#include "MeshLambertMaterial.h"

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
            case MESH_LAMBERT_MATERIAL:
                if (auto lambertMaterial = dynamic_cast<MeshLambertMaterial *>(material)) {

                    NSError *err;

                    NSString *libPath = [[NSBundle mainBundle] pathForResource:@"RenderBoxMetalLibrary" ofType:@"metallib"];
                    id <MTLLibrary> library = [device newLibraryWithFile:libPath error:&err];
                    assert(err == nil);

                    MTLRenderPipelineDescriptor *renderPipelineDescriptor = [[MTLRenderPipelineDescriptor alloc] init];

                    NSString *vertexFunctionName = @"lambert_vert";
                    NSString *fragmentFunctionName = @"lambert_frag";

                    auto ambientMap = lambertMaterial->getAmbientMap();
                    auto diffuseMap = lambertMaterial->getDiffuseMap();

                    if (ambientMap || diffuseMap) {
                        vertexFunctionName = [vertexFunctionName stringByAppendingString:@"_uv"];
                        if (ambientMap) fragmentFunctionName = [fragmentFunctionName stringByAppendingString:@"_ambientmap"];
                        if (diffuseMap) fragmentFunctionName = [fragmentFunctionName stringByAppendingString:@"_diffusemap"];
                    }

                    id <MTLFunction> vertexFunction = [library newFunctionWithName:vertexFunctionName];
                    id <MTLFunction> fragmentFunction = [library newFunctionWithName:fragmentFunctionName];

                    [renderPipelineDescriptor setVertexFunction:vertexFunction];
                    [renderPipelineDescriptor setFragmentFunction:fragmentFunction];

                    renderPipelineDescriptor.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;
                    renderPipelineDescriptor.depthAttachmentPixelFormat = MTLPixelFormatDepth32Float;

                    renderPipelineStateObject = [device newRenderPipelineStateWithDescriptor:renderPipelineDescriptor
                                                                                       error:&err];
                    assert(err == nil);

                } else {
                    throw;
                };
                break;
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

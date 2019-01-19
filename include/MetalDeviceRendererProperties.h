#ifndef RENDERBOX_METALDEVICERENDERERPROPERTIES_H
#define RENDERBOX_METALDEVICERENDERERPROPERTIES_H


#import <Metal/Metal.h>

#include "MetalObjectProperties.h"
#include "MetalRenderPipelineState.h"
#include "Object.h"
#include "VersionTrackedObject.h"


namespace renderbox {

    struct ObjectMetalRenderPipelineState {
        std::shared_ptr<MetalRenderPipelineState> renderPipelineState;
        VersionTrackedObject::version_type geometryVersion;
        VersionTrackedObject::version_type materialVersion;
    };

    class MetalDeviceRendererProperties {

        friend class MetalRenderer;

    public:

        explicit MetalDeviceRendererProperties(id <MTLDevice> device);

        MetalRenderPipelineState *getRenderPipelineState(Object *object);

        MetalObjectProperties *getObjectProperties(Object *object, bool *blankObjectProperties = nullptr);

        MTLRenderPassDescriptor *getRenderPassDescriptor(id <MTLTexture> targetTexture);

        id <MTLDepthStencilState> getDepthStencilState();

    protected:

        id <MTLDevice> device;

    private:

        id <MTLLibrary> defaultMetalLibrary_;

        std::unordered_map<int, std::unique_ptr<MetalObjectProperties>> objectProperties_;

        std::unordered_map<std::string, std::shared_ptr<MetalRenderPipelineState>> cachedRenderPipelineStates_;

        std::unordered_map<Object *, ObjectMetalRenderPipelineState> renderPipelineStates_;

        id <MTLLibrary> getDefaultMetalLibrary();

        std::shared_ptr<MetalRenderPipelineState>
        getRenderPipelineStateWithFunctionNames(std::string const &vertexFunctionName,
                                                std::string const &fragmentFunctionName);

        scoped_nsobject<MTLRenderPassDescriptor> renderPassDescriptor;

        NSUInteger depthAttachmentTextureWidth;
        NSUInteger depthAttachmentTextureHeight;

        scoped_nsprotocol<id <MTLDepthStencilState>> depthStencilState;

    };

}


#endif //RENDERBOX_METALDEVICERENDERERPROPERTIES_H

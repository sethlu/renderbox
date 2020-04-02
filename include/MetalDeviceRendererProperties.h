#ifndef RENDERBOX_METALDEVICERENDERERPROPERTIES_H
#define RENDERBOX_METALDEVICERENDERERPROPERTIES_H


#import <Metal/Metal.h>

#include "std.h"
#include "MetalObjectProperties.h"
#include "MetalRenderPipelineState.h"
#include "Object.h"
#include "VersionTrackedObject.h"


namespace renderbox {

    struct ObjectMetalRenderPipelineState {
        std::shared_ptr<MetalRenderPipelineState> renderPipelineState;
        VersionTrackedObject::version_type materialVersion;
        VersionTrackedObject::version_type geometryVersion;
    };

    class MetalDeviceRendererProperties {

        friend class MetalRenderer;

    public:

        explicit MetalDeviceRendererProperties(id <MTLDevice> device);

        MetalRenderPipelineState *getRenderPipelineState(Material *material, Geometry *geometry);

        MetalObjectProperties *getObjectProperties(Object *object, bool *blankObjectProperties = nullptr);

        MTLRenderPassDescriptor *getRenderPassDescriptor(id <MTLTexture> targetTexture);

        id <MTLDepthStencilState> getDepthStencilState();

    protected:

        id <MTLDevice> device;

    private:

        id <MTLLibrary> _defaultMetalLibrary;

        std::unordered_map<int, std::unique_ptr<MetalObjectProperties>> _objectProperties;

        std::unordered_map<std::string, std::shared_ptr<MetalRenderPipelineState>>
                _cachedRenderPipelineStates;

        std::unordered_map<std::pair<Material *, Geometry *>, ObjectMetalRenderPipelineState>
                _renderPipelineStates;

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

#include "MetalDeviceRendererProperties.h"

#include "MeshGeometry.h"
#include "MeshBasicMaterial.h"
#include "MeshLambertMaterial.h"
#include "logging.h"
#include "platform.h"

namespace renderbox {

    MetalDeviceRendererProperties::MetalDeviceRendererProperties(id <MTLDevice> device)
        : device(device) {

        // Render pass descriptor

        renderPassDescriptor.reset([MTLRenderPassDescriptor renderPassDescriptor]);

        // Color attachment
        MTLRenderPassColorAttachmentDescriptor *renderPassColorAttachment = [renderPassDescriptor colorAttachments][0];
        renderPassColorAttachment.loadAction = MTLLoadActionClear;
        renderPassColorAttachment.clearColor = MTLClearColorMake(0, 0, 0, 1);
        renderPassColorAttachment.storeAction = MTLStoreActionStore;

        // Depth attachment
        MTLRenderPassDepthAttachmentDescriptor *renderPassDepthAttachment = [renderPassDescriptor depthAttachment];
        renderPassDepthAttachment.loadAction = MTLLoadActionClear;
        renderPassDepthAttachment.clearDepth = 1.0f;
        renderPassDepthAttachment.storeAction = MTLStoreActionDontCare;

        // Depth stencil

        MTLDepthStencilDescriptor *depthStencilDescriptor = [MTLDepthStencilDescriptor new];
        depthStencilDescriptor.depthCompareFunction = MTLCompareFunctionLess;
        depthStencilDescriptor.depthWriteEnabled = YES;

        depthStencilState.reset([device newDepthStencilStateWithDescriptor:depthStencilDescriptor]);

    }

    id <MTLLibrary>
    MetalDeviceRendererProperties::getDefaultMetalLibrary() {
        if (!_defaultMetalLibrary) {
            NSError *err;

#ifdef RENDERBOX_OS_MACOS
            auto *libPath = [[NSBundle mainBundle] pathForResource:@"RenderBoxMetalLibrary" ofType:@"metallib"];
#else
            auto *libPath = [[NSBundle mainBundle] pathForResource:@"RenderBoxMetalLibrary-iOS" ofType:@"metallib"];
#endif

            _defaultMetalLibrary = [device newLibraryWithFile:libPath error:&err];
            if (!_defaultMetalLibrary) {
                LOG(ERROR) << "Failed to load Metal library." << std::endl;
                NSLog(@"%@", err);
                exit(EXIT_FAILURE);
            }
        }
        return _defaultMetalLibrary;
    }

    std::shared_ptr<MetalRenderPipelineState>
    MetalDeviceRendererProperties
    ::getRenderPipelineStateWithFunctionNames(std::string const &vertexFunctionName,
                                              std::string const &fragmentFunctionName) {

        auto const &key = vertexFunctionName + "$" + fragmentFunctionName;

        auto result = _cachedRenderPipelineStates.find(key);
        if (result != _cachedRenderPipelineStates.end()) {
            return result->second;
        }

        LOG(VERBOSE) << "getRenderPipelineStateWithFunctionNames: MISS " << key << std::endl;

        auto library = getDefaultMetalLibrary();

        auto vertexFunction = [library newFunctionWithName:[NSString stringWithUTF8String:vertexFunctionName.c_str()]];
        auto fragmentFunction = [library newFunctionWithName:[NSString stringWithUTF8String:fragmentFunctionName.c_str()]];

        // Pipeline descriptor

        MTLRenderPipelineDescriptor *renderPipelineDescriptor = [[MTLRenderPipelineDescriptor alloc] init];

        [renderPipelineDescriptor setVertexFunction:vertexFunction];
        [renderPipelineDescriptor setFragmentFunction:fragmentFunction];

        renderPipelineDescriptor.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;
        renderPipelineDescriptor.depthAttachmentPixelFormat = MTLPixelFormatDepth32Float;

        // Pipeline state

        NSError *err;
        auto renderPipelineStateObject =
            [device newRenderPipelineStateWithDescriptor:renderPipelineDescriptor
                                                   error:&err];
        if (!renderPipelineStateObject) {
            LOG(ERROR) << "Failed to create Metal render pipeline state." << std::endl;
            NSLog(@"%@", err);
            exit(EXIT_FAILURE);
        }


        auto renderPipelineState = std::make_shared<MetalRenderPipelineState>(renderPipelineStateObject);
        _cachedRenderPipelineStates.insert(std::make_pair(key, renderPipelineState));

        return renderPipelineState;

    }

    MetalRenderPipelineState *
    MetalDeviceRendererProperties::getRenderPipelineState(Material *material, Geometry *geometry) {
        auto const &metalMaterial = dynamic_cast<MetalMaterial *>(material);
        if (!metalMaterial) {
            LOG(ERROR) << "Unsupported material type" << std::endl;
            exit(EXIT_FAILURE);
        };

        auto const &materialGeometryPair = std::make_pair(material, geometry);

        ObjectMetalRenderPipelineState *objectMetalRenderPipelineState = nullptr;

        auto result = _renderPipelineStates.find(materialGeometryPair);
        if (result != _renderPipelineStates.end()) {
            if (result->second.materialVersion == material->getVersion() &&
                result->second.geometryVersion == geometry->getVersion()) {
                return result->second.renderPipelineState.get();
            } else {
                LOG(VERBOSE) << "getRenderPipelineState: MISS (invalidated)" << std::endl;
                objectMetalRenderPipelineState = &result->second;
            }
        } else {
            LOG(VERBOSE) << "getRenderPipelineState: MISS (cold)" << std::endl;
        }

        std::shared_ptr<MetalRenderPipelineState> renderPipelineState =
                getRenderPipelineStateWithFunctionNames(
                        metalMaterial->getMetalVertexFunctionName(geometry),
                        metalMaterial->getMetalFragmentFunctionName(geometry));

        if (objectMetalRenderPipelineState) {
            objectMetalRenderPipelineState->renderPipelineState = renderPipelineState;
            objectMetalRenderPipelineState->materialVersion = material->getVersion();
            objectMetalRenderPipelineState->geometryVersion = geometry->getVersion();
        } else {
            _renderPipelineStates.insert(std::make_pair(materialGeometryPair, (ObjectMetalRenderPipelineState) {
                renderPipelineState,
                material->getVersion(),
                geometry->getVersion()
            }));
        }

        return renderPipelineState.get();

    }

    MetalObjectProperties *
    MetalDeviceRendererProperties::getObjectProperties(Object *object, bool *blankObjectProperties) {

        auto result = _objectProperties.find(object->getObjectId());
        if (result != _objectProperties.end()) {
            if (blankObjectProperties) *blankObjectProperties = false;
            return result->second.get();
        }

        auto *properties = new MetalObjectProperties(device);
        _objectProperties.insert(std::make_pair(object->getObjectId(),
                                                std::unique_ptr<MetalObjectProperties>(properties)));

        if (blankObjectProperties) *blankObjectProperties = true;
        return properties;

    }

    MTLRenderPassDescriptor *MetalDeviceRendererProperties::getRenderPassDescriptor(id <MTLTexture> targetTexture) {
        [renderPassDescriptor colorAttachments][0].texture = targetTexture;

        if (targetTexture.width != depthAttachmentTextureWidth ||
            targetTexture.height != depthAttachmentTextureHeight) {

            depthAttachmentTextureWidth = targetTexture.width;
            depthAttachmentTextureHeight = targetTexture.height;

            // Depth attachment texture
            MTLTextureDescriptor *depthAttachmentTextureDescriptor =
            [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatDepth32Float
                                                               width:depthAttachmentTextureWidth
                                                              height:depthAttachmentTextureHeight
                                                           mipmapped:NO];
            depthAttachmentTextureDescriptor.resourceOptions = MTLResourceStorageModePrivate;
            depthAttachmentTextureDescriptor.usage = MTLTextureUsageRenderTarget;

            [renderPassDescriptor depthAttachment].texture = [device newTextureWithDescriptor:depthAttachmentTextureDescriptor];

        }

        return renderPassDescriptor;
    }

    id <MTLDepthStencilState> MetalDeviceRendererProperties::getDepthStencilState() {
        return depthStencilState;
    }

}

#include "MetalDeviceRendererProperties.h"
#include "MeshBasicMaterial.h"
#include "MeshLambertMaterial.h"
#include "logging.h"
#include "platform.h"

namespace renderbox {

    MetalDeviceRendererProperties::MetalDeviceRendererProperties(id <MTLDevice> device)
        : device(device) {

    }

    id <MTLLibrary>
    MetalDeviceRendererProperties::getDefaultMetalLibrary() {
        if (!defaultMetalLibrary_) {
            NSError *err;

#ifdef RENDERBOX_OS_MACOS
            auto *libPath = [[NSBundle mainBundle] pathForResource:@"RenderBoxMetalLibrary" ofType:@"metallib"];
#else
            auto *libPath = [[NSBundle mainBundle] pathForResource:@"RenderBoxMetalLibrary-iOS" ofType:@"metallib"];
#endif

            defaultMetalLibrary_ = [device newLibraryWithFile:libPath error:&err];
            if (!defaultMetalLibrary_) {
                LOG(ERROR) << "Failed to load Metal library." << std::endl;
                NSLog(@"%@", err);
                exit(EXIT_FAILURE);
            }
        }
        return defaultMetalLibrary_;
    }

    std::shared_ptr<MetalRenderPipelineState>
    MetalDeviceRendererProperties
    ::getRenderPipelineStateWithFunctionNames(std::string const &vertexFunctionName,
                                              std::string const &fragmentFunctionName) {

        auto const &key = vertexFunctionName + "$" + fragmentFunctionName;

        auto result = cachedRenderPipelineStates_.find(key);
        if (result != cachedRenderPipelineStates_.end()) {
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
        cachedRenderPipelineStates_.insert(std::make_pair(key, renderPipelineState));

        return renderPipelineState;

    }

    MetalRenderPipelineState *
    MetalDeviceRendererProperties::getRenderPipelineState(Object *object) {
        auto const &geometry = object->getGeometry();
        auto const &material = object->getMaterial();

        auto result = renderPipelineStates_.find(object);
        if (result != renderPipelineStates_.end()) {
            if (result->second.geometryVersion == geometry->getVersion() &&
                result->second.materialVersion == material->getVersion()) {
                return result->second.renderPipelineState.get();
            } else {
                LOG(VERBOSE) << "getRenderPipelineState: MISS (invalidated) object=" << object->getObjectId() << std::endl;
            }
        } else {
            LOG(VERBOSE) << "getRenderPipelineState: MISS (cold) object=" << object->getObjectId() << std::endl;
        }

        // Make new render pipeline descriptor

        std::shared_ptr<MetalRenderPipelineState> renderPipelineState;

        switch (object->getMaterial()->getMaterialType()) {
            default:
                LOG(FATAL) << "Unsupported material type." << std::endl;
                exit(EXIT_FAILURE);
                break;
            case MESH_BASIC_MATERIAL:
                if (auto basicMaterial = std::dynamic_pointer_cast<MeshBasicMaterial>(material)) {

                    std::string vertexFunctionName = "mesh_basic_vert";
                    std::string fragmentFunctionName = "mesh_basic_frag";

                    if (!geometry->skinIndices.empty() && !geometry->skinWeights.empty()) {
                        vertexFunctionName += "_skin";
                    }

                    auto diffuseMap = basicMaterial->getDiffuseMap();

                    if (diffuseMap) {
                        vertexFunctionName += "_uv";
                        fragmentFunctionName += "_diffusemap";
                    }

                    renderPipelineState =
                        getRenderPipelineStateWithFunctionNames(vertexFunctionName,
                                                                fragmentFunctionName);

                } else {
                    NOTREACHED();
                }
                break;
            case MESH_LAMBERT_MATERIAL:
                if (auto lambertMaterial = std::dynamic_pointer_cast<MeshLambertMaterial>(material)) {

                    std::string vertexFunctionName = "mesh_lambert_vert";
                    std::string fragmentFunctionName = "mesh_lambert_frag";

                    if (!geometry->skinIndices.empty() && !geometry->skinWeights.empty()) {
                        vertexFunctionName += "_skin";
                    }

                    auto ambientMap = lambertMaterial->getAmbientMap();
                    auto diffuseMap = lambertMaterial->getDiffuseMap();

                    if (ambientMap || diffuseMap) {
                        vertexFunctionName += "_uv";
                        if (ambientMap) fragmentFunctionName += "_ambientmap";
                        if (diffuseMap) fragmentFunctionName += "_diffusemap";
                    }

                    renderPipelineState =
                        getRenderPipelineStateWithFunctionNames(vertexFunctionName,
                                                                fragmentFunctionName);

                } else {
                    NOTREACHED();
                }
                break;
        }

        renderPipelineStates_.insert(std::make_pair(object, (ObjectMetalRenderPipelineState) {
            renderPipelineState,
            geometry->getVersion(),
            material->getVersion()
        }));

        return renderPipelineState.get();

    }

    MetalObjectProperties *
    MetalDeviceRendererProperties::getObjectProperties(Object *object, bool *blankObjectProperties) {

        auto result = objectProperties_.find(object->getObjectId());
        if (result != objectProperties_.end()) {
            if (blankObjectProperties) *blankObjectProperties = false;
            return result->second.get();
        }

        auto *properties = new MetalObjectProperties(device);
        objectProperties_.insert(std::make_pair(object->getObjectId(),
                                                std::unique_ptr<MetalObjectProperties>(properties)));

        if (blankObjectProperties) *blankObjectProperties = true;
        return properties;

    }

}

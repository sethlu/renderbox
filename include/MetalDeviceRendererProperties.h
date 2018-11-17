#ifndef RENDERBOX_METALDEVICERENDERERPROPERTIES_H
#define RENDERBOX_METALDEVICERENDERERPROPERTIES_H


#import <Metal/Metal.h>

#include "MetalObjectProperties.h"
#include "MetalRenderPipelineState.h"
#include "Object.h"


namespace renderbox {

    class MetalDeviceRendererProperties {

        friend class MetalRenderer;

    public:

        MetalDeviceRendererProperties(id <MTLDevice> device);

        MetalRenderPipelineState *getRenderPipelineState(Material *material);

        MetalObjectProperties *getObjectProperties(Object *object, bool *blankObjectProperties = nullptr);

    protected:

        id <MTLDevice> device;

        std::unordered_map<int, std::unique_ptr<MetalObjectProperties>> objectProperties;

        std::unordered_map<int, std::unique_ptr<MetalRenderPipelineState>> renderPipelineStates;

    };

}


#endif //RENDERBOX_METALDEVICERENDERERPROPERTIES_H

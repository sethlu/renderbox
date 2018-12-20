#ifndef RENDERBOX_METALRENDERERPROPERTIES_H
#define RENDERBOX_METALRENDERERPROPERTIES_H


#include <unordered_map>

#include "MetalDeviceRendererProperties.h"

namespace renderbox {

    class MetalRendererProperties {

        friend class MetalRenderer;

    public:

        std::unordered_map<const void *, std::unique_ptr<MetalDeviceRendererProperties>> deviceRendererProperties;

        MetalDeviceRendererProperties *getDeviceRendererProperties(id <MTLDevice> device);

    };

}


#endif //RENDERBOX_METALRENDERERPROPERTIES_H

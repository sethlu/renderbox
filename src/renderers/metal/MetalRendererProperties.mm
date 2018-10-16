#include "MetalRendererProperties.h"


namespace renderbox {

    MetalDeviceRendererProperties *MetalRendererProperties::getDeviceRendererProperties(id<MTLDevice> device) {

        const void *_device = (__bridge const void *) device;

        auto result = deviceRendererProperties.find(_device);
        if (result != deviceRendererProperties.end()) {
            return result->second.get();
        }

        auto properties = new MetalDeviceRendererProperties(device);
        deviceRendererProperties.insert(
                std::make_pair(_device, std::unique_ptr<MetalDeviceRendererProperties>(properties)));
        return properties;

    }

}

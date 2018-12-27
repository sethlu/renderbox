#ifndef RENDERBOX_METALOBJECTPROPERTIES_H
#define RENDERBOX_METALOBJECTPROPERTIES_H


#include <unordered_map>

#include "ObjectProperties.h"
#include "MetalBuffer.h"
#include "MetalTexture.h"
#include "VersionTrackedObject.h"

namespace renderbox {

    class MetalObjectProperties : public ObjectProperties {
    public:

        explicit MetalObjectProperties(id <MTLDevice> device);

        MetalBuffer *getBuffer(int index);

        MetalTexture *getTexture(int index, bool *blankTexture);

        VersionTrackedObject::version_type geometryVersion;

        id <MTLBuffer> uniformBuffer;

    protected:

        id <MTLDevice> device;

        std::unordered_map<int, std::unique_ptr<MetalBuffer>> buffers;

        std::unordered_map<int, std::unique_ptr<MetalTexture>> textures;

    };

}


#endif //RENDERBOX_METALOBJECTPROPERTIES_H

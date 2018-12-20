#ifndef RENDERBOX_METALRENDERPIPELINESTATE_H
#define RENDERBOX_METALRENDERPIPELINESTATE_H


#include <Metal/Metal.h>

#include "scoped_nsobject.h"

namespace renderbox {

    class MetalRenderPipelineState {

        friend class MetalRenderer;

    public:

        explicit MetalRenderPipelineState(id <MTLRenderPipelineState> renderPipelineState);

    private:

        scoped_nsprotocol<id <MTLRenderPipelineState>> renderPipelineStateObject;

    };

}


#endif //RENDERBOX_METALRENDERPIPELINESTATE_H

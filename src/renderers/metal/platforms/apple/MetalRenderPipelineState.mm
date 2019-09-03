#include "MetalRenderPipelineState.h"


namespace renderbox {

    MetalRenderPipelineState::MetalRenderPipelineState(id <MTLRenderPipelineState> renderPipelineState)
            : renderPipelineStateObject(scoped_nsprotocol<id <MTLRenderPipelineState>> (renderPipelineState)) {}

}

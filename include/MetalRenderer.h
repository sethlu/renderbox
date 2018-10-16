#ifndef RENDERBOX_METALRENDERER_H
#define RENDERBOX_METALRENDERER_H


#include <unordered_map>

#import <Metal/Metal.h>

#include "Camera.h"
#include "Scene.h"
#include "MetalBuffer.h"
#include "MetalRendererProperties.h"
#include "SDLMetalRenderTarget.h"


namespace renderbox {

    class MetalRenderer {
    public:

        void render(Scene *scene, Camera *camera, SDLMetalRenderTarget *renderTarget);

    protected:

        MetalRendererProperties properties;

    };

}


#endif //RENDERBOX_METALRENDERER_H

#ifndef RENDERBOX_OPENGLSDLRENDERER_H
#define RENDERBOX_OPENGLSDLRENDERER_H


#include <SDL2/SDL.h>
#include "OpenGLRenderer.h"

namespace renderbox {

    class OpenGLSDLRenderer : public OpenGLRenderer {
    protected:

        SDL_Window *window;

        SDL_GLContext context;

    public:

        OpenGLSDLRenderer(Uint32 additionalInitFlags = 0);

        ~OpenGLSDLRenderer();

        SDL_Window *getWindow() const;

        int getFramebufferWidth() const override;

        int getFramebufferHeight() const override;

    };

}

#endif //RENDERBOX_OPENGLSDLRENDERER_H

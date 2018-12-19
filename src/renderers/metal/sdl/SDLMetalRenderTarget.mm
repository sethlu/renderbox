#import "platform.h"
#import "SDLMetalRenderTarget.h"

#include <SDL_syswm.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

@implementation MetalView

- (CALayer *)makeBackingLayer {
    return [[CAMetalLayer alloc] init];
}

- (instancetype)initWithFrame:(NSRect)frame {
    if (self = [super initWithFrame:frame]) {
        self.wantsLayer = YES;

        _metalLayer = (CAMetalLayer *) self.layer;
        _metalLayer.device = MTLCreateSystemDefaultDevice();
        _metalLayer.pixelFormat = MTLPixelFormatBGRA8Unorm;
        _metalLayer.framebufferOnly = true;
    }

    return self;
}

@end

namespace renderbox {

    SDLMetalRenderTarget::SDLMetalRenderTarget() {

        // SDL

        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            fprintf(stderr, "Failed to initialize SDL\n");
            throw 2;
        }

        // Create window

#if defined(RENDERBOX_OS_MACOS)
        window = SDL_CreateWindow(nullptr, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                  WINDOW_WIDTH, WINDOW_HEIGHT,
                                  SDL_WINDOW_ALLOW_HIGHDPI);
#elif defined(RENDERBOX_OS_IPHONEOS)
        SDL_DisplayMode displayMode;
        SDL_GetDesktopDisplayMode(0, &displayMode);
        
        window = SDL_CreateWindow(nullptr, 0, 0,
                displayMode.w, displayMode.h,
                SDL_WINDOW_BORDERLESS | SDL_WINDOW_ALLOW_HIGHDPI);
#endif

        if (!window) {
            fprintf(stderr, "Failed to init window\n");
            throw 2;
        }

        // Set up Metal

        // Get SDL view
        SDL_SysWMinfo info;
        SDL_VERSION(&info.version);
        SDL_GetWindowWMInfo(window, &info);
        NSView *sdlView = info.info.cocoa.window.contentView;

        // Create metal view
        metalView = [[MetalView alloc] initWithFrame:sdlView.frame];
        [sdlView addSubview:metalView];

        // Create command queue
        queue = [metalView.metalLayer.device newCommandQueue];

    }

    SDLMetalRenderTarget::~SDLMetalRenderTarget() {

        SDL_GL_DeleteContext(context);
        SDL_DestroyWindow(window);

    }

    SDL_Window *SDLMetalRenderTarget::getWindow() const {
        return window;
    }

    int SDLMetalRenderTarget::getFramebufferWidth() const {
        int width;
        SDL_GL_GetDrawableSize(window, &width, nullptr);
        return width;
    }

    int SDLMetalRenderTarget::getFramebufferHeight() const {
        int height;
        SDL_GL_GetDrawableSize(window, nullptr, &height);
        return height;
    }

}

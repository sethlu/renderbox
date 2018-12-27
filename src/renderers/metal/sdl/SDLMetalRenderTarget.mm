#import "SDLMetalRenderTarget.h"
#import "platform.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600


#ifdef RENDERBOX_OS_MACOS

@implementation MetalView

- (CALayer *)makeBackingLayer {
    return [[CAMetalLayer alloc] init];
}

- (instancetype)initWithFrame:(NSRect)frame scale:(CGFloat)scale {
    if (self = [super initWithFrame:frame]) {
        self.wantsLayer = YES;
        self.autoresizingMask = NSViewWidthSizable | NSViewHeightSizable;
        self.layer.contentsScale = scale;

        _metalLayer = (CAMetalLayer *) self.layer;
        _metalLayer.device = MTLCreateSystemDefaultDevice();
        _metalLayer.pixelFormat = MTLPixelFormatBGRA8Unorm;
        _metalLayer.framebufferOnly = true;

        [self updateMetalLayerDrawableSize];
    }

    return self;
}

- (void)setFrameSize:(NSSize)newSize {
    [super setFrameSize:newSize];
    [self updateMetalLayerDrawableSize];
}

- (void)updateMetalLayerDrawableSize {
    CGSize size = self.layer.bounds.size;
    size.width *= self.layer.contentsScale;
    size.height *= self.layer.contentsScale;
    _metalLayer.drawableSize = size;
}

@end

#else

@implementation MetalView

+ (Class)layerClass {
    return [CAMetalLayer class];
}

- (instancetype)initWithFrame:(CGRect)frame scale:(CGFloat)scale {
    if (self = [super initWithFrame:frame]) {
        self.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
        self.contentScaleFactor = scale;

        _metalLayer = (CAMetalLayer *) self.layer;
        _metalLayer.device = MTLCreateSystemDefaultDevice();
        _metalLayer.pixelFormat = MTLPixelFormatBGRA8Unorm;
        _metalLayer.framebufferOnly = true;

        [self updateDrawableSize];
    }

    return self;
}

- (void)layoutSubviews {
    [super layoutSubviews];
    [self updateDrawableSize];
}

- (void)updateDrawableSize {
    CGSize size = self.bounds.size;
    size.width *= self.contentScaleFactor;
    size.height *= self.contentScaleFactor;
    _metalLayer.drawableSize = size;
}

@end

#endif


namespace renderbox {

    SDLMetalRenderTarget::SDLMetalRenderTarget() {

        // SDL

        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            fprintf(stderr, "Failed to initialize SDL\n");
            throw 2;
        }

        // Create window

#if RENDERBOX_OS_MACOS
        sdlWindow = SDL_CreateWindow(nullptr, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                     WINDOW_WIDTH, WINDOW_HEIGHT,
                                     SDL_WINDOW_ALLOW_HIGHDPI);
#else
        SDL_DisplayMode displayMode;
        SDL_GetDesktopDisplayMode(0, &displayMode);
        
        sdlWindow = SDL_CreateWindow(nullptr, 0, 0,
                                     displayMode.w, displayMode.h,
                                     SDL_WINDOW_BORDERLESS | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
#endif

        if (!sdlWindow) {
            fprintf(stderr, "Failed to init window\n");
            throw 2;
        }

        // Set up Metal

        // Get SDL view
        SDL_VERSION(&info.version);
        SDL_GetWindowWMInfo(sdlWindow, &info);

        // Create Metal view

#ifdef RENDERBOX_OS_MACOS
        sdlView = info.info.cocoa.window.contentView;
        metalView = [[MetalView alloc] initWithFrame:sdlView.frame
                                               scale:info.info.cocoa.window.backingScaleFactor];
#else
        sdlView = info.info.uikit.window.rootViewController.view;
        metalView = [[MetalView alloc] initWithFrame:sdlView.frame scale:[UIScreen mainScreen].nativeScale];
#endif

        [sdlView addSubview:metalView];

        // Keep Metal device
        metalDevice = metalView.metalLayer.device;

        // Create command queue
        metalQueue.reset([metalDevice newCommandQueue]);

    }

    SDLMetalRenderTarget::~SDLMetalRenderTarget() {

        SDL_GL_DeleteContext(context);
        SDL_DestroyWindow(sdlWindow);

    }

    SDL_Window *SDLMetalRenderTarget::getWindow() const {
        return sdlWindow;
    }

    int SDLMetalRenderTarget::getWindowWidth() const {
#if RENDERBOX_OS_MACOS
        return info.info.cocoa.window.contentView.frame.size.width;
#else
        return info.info.uikit.window.rootViewController.view.frame.size.width;
#endif
    }

    int SDLMetalRenderTarget::getWindowHeight() const {
        SDL_SysWMinfo info;
        SDL_VERSION(&info.version);
        SDL_GetWindowWMInfo(sdlWindow, &info);

#if RENDERBOX_OS_MACOS
        return info.info.cocoa.window.contentView.frame.size.height;
#else
        return info.info.uikit.window.rootViewController.view.frame.size.height;
#endif
    }

    int SDLMetalRenderTarget::getFramebufferWidth() const {
        return ((CAMetalLayer *) metalView.layer).drawableSize.width;
    }

    int SDLMetalRenderTarget::getFramebufferHeight() const {
        return ((CAMetalLayer *) metalView.layer).drawableSize.height;
    }

}

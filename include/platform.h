#ifndef RENDERBOX_PLATFORM_H
#define RENDERBOX_PLATFORM_H


#include "AvailabilityMacros.h"
#include "TargetConditionals.h"

#if TARGET_OS_IPHONE
#define RENDERBOX_IPHONEOS 1
#else
#define RENDERBOX_MACOS  1
#endif


#endif //RENDERBOX_PLATFORM_H

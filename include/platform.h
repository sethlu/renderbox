#ifndef RENDERBOX_PLATFORM_H
#define RENDERBOX_PLATFORM_H


#if __APPLE__

#include "AvailabilityMacros.h"
#include "TargetConditionals.h"

#if TARGET_OS_IPHONE
#define RENDERBOX_OS_IPHONEOS
#else
#define RENDERBOX_OS_MACOS
#endif

#elif __linux__

#define RENDERBOX_OS_LINUX

#endif


#endif //RENDERBOX_PLATFORM_H

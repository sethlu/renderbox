#ifndef RENDERBOX_PLATFORM_H
#define RENDERBOX_PLATFORM_H


/* lets us know what version of Mac OS X we're compiling on */
#include "AvailabilityMacros.h"
#include "TargetConditionals.h"

#if TARGET_OS_IPHONE
/* if compiling for iPhone */
#undef __IPHONEOS__
#define __IPHONEOS__ 1
#undef __MACOSX__
#else
/* if not compiling for iPhone */
#undef __MACOSX__
#define __MACOSX__  1
#endif /* TARGET_OS_IPHONE */


#endif //RENDERBOX_PLATFORM_H

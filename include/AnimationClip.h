#ifndef RENDERBOX_ANIMATIONCLIP_H
#define RENDERBOX_ANIMATIONCLIP_H


#include <string>
#include <vector>

#include "KeyframeTrack.h"


namespace renderbox {

    class AnimationClip {
    public:

        float duration;

        std::vector<std::shared_ptr<KeyframeTrack>> tracks;

    };

}


#endif //RENDERBOX_ANIMATIONCLIP_H

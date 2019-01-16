#ifndef RENDERBOX_ANIMATIONACTION_H
#define RENDERBOX_ANIMATIONACTION_H


#include <unordered_map>

#include "AnimationClip.h"
#include "PropertyMixer.h"


namespace renderbox {

    class AnimationAction {
    public:

        std::shared_ptr<AnimationClip> clip;

        std::unordered_map<PropertyMixer *, size_t> bindings;

        float weight = 1.f;

        void bind(PropertyMixer *propertyMixer, size_t clipTrackIndex);

    };

}


#endif //RENDERBOX_ANIMATIONACTION_H

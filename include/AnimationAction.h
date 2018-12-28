#ifndef RENDERBOX_ANIMATIONACTION_H
#define RENDERBOX_ANIMATIONACTION_H


#include <unordered_map>

#include "AnimationClip.h"
#include "PropertyMixer.h"


namespace renderbox {

    class AnimationAction {
    public:

        std::shared_ptr<AnimationClip> clip;

        std::unordered_map<PropertyMixer *, int> bindings;

        float weight = 1.f;

        void bind(PropertyMixer *propertyMixer, int clipTrackIndex);

    };

}


#endif //RENDERBOX_ANIMATIONACTION_H

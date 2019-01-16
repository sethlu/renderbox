#include "AnimationAction.h"

#include "Scene.h"


namespace renderbox {

    void AnimationAction::bind(PropertyMixer *propertyMixer, size_t clipTrackIndex) {
        bindings.insert(std::make_pair(propertyMixer, clipTrackIndex));
    }

}

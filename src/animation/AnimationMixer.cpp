#include "AnimationMixer.h"

#include <cmath>

#include "logging.h"


namespace renderbox {

    void AnimationMixer::update(float t) {
        for (auto propertyMixer : propertyMixers) {
            propertyMixer->clear();
        }

        for (auto &action : actions) {
            auto const &tracks = action->clip->tracks;
            auto weight = action->weight;

            for (auto pair : action->bindings) {
                auto &propertyMixer = pair.first;
                auto &trackIndex = pair.second;
                auto track = tracks[trackIndex];

                switch (propertyMixer->getObjectProperty()) {
                    default: {
                        NOTREACHED() << "Unhandled object property";
                        exit(EXIT_FAILURE);
                    }
                    case OBJECT_PROPERTY_TRANSLATION: {
                        auto translationPropertyMixer = dynamic_cast<TranslationPropertyMixer *>(propertyMixer);
                        auto vector3KeyframeTrack = dynamic_cast<Vector3KeyframeTrack *>(track.get());
                        if (translationPropertyMixer && vector3KeyframeTrack) {
                            translationPropertyMixer->accumulate(
                                    vector3KeyframeTrack->interpolate(fmod(t, action->clip->duration)), weight);
                        } else {
                            NOTREACHED();
                        }
                    }
                }
            }
        }

        for (auto propertyMixer : propertyMixers) {
            propertyMixer->apply();
        }
    }

    PropertyMixer *AnimationMixer::getPropertyMixer(std::shared_ptr<Object> object, OBJECT_PROPERTY property) {
        auto result = objectPropertyMixers.find(object.get());
        ObjectPropertyMixers *mixers;
        if (result != objectPropertyMixers.end()) {
            mixers = result->second.get();
        } else {
            mixers = new ObjectPropertyMixers();
            objectPropertyMixers.insert(std::make_pair(object.get(), mixers));
        }

        switch (property) {
            default: {
                NOTREACHED() << "Unhandled object property";
                exit(EXIT_FAILURE);
            }
            case OBJECT_PROPERTY_TRANSLATION: {
                if (mixers->translationPropertyMixer) {
                    return mixers->translationPropertyMixer.get();
                }
                auto newMixer = new TranslationPropertyMixer(object);
                mixers->translationPropertyMixer.reset(newMixer);
                propertyMixers.insert(newMixer);
                return newMixer;
            }
        }
    }

    void AnimationMixer::removePropertyMixers(Object *object) {
        auto it = objectPropertyMixers.find(object);
        if (it == objectPropertyMixers.end()) return;

        auto &mixers = it->second;
        if (mixers->translationPropertyMixer) propertyMixers.erase(mixers->translationPropertyMixer.get());
        objectPropertyMixers.erase(it);
    }

}

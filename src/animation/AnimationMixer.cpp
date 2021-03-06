#include "AnimationMixer.h"

#include <cmath>

#include "logging.h"


namespace renderbox {

    void AnimationMixer::update(float t) {
        for (auto const &propertyMixer : propertyMixers) {
            propertyMixer->clear();
        }

        for (auto const &action : actions) {
            auto const &tracks = action->clip->tracks;
            auto weight = action->weight;

            for (auto const &pair : action->bindings) {
                auto &propertyMixer = pair.first;
                auto &trackIndex = pair.second;
                auto track = tracks[trackIndex];

                auto floatPropertyMixer = dynamic_cast<AccumulableFloatPropertyMixer *>(propertyMixer);
                auto vector3PropertyMixer = dynamic_cast<AccumulableVector3PropertyMixer *>(propertyMixer);

                auto vector3KeyframeTrack = dynamic_cast<Vector3KeyframeTrack *>(track.get());
                auto floatKeyframeTrack = dynamic_cast<FloatKeyframeTrack *>(track.get());

                if (floatPropertyMixer && floatKeyframeTrack) {
                    floatPropertyMixer->accumulate(floatKeyframeTrack->interpolate(fmod(t, action->clip->duration)), weight);
                } else if (vector3PropertyMixer && vector3KeyframeTrack) {
                    vector3PropertyMixer->accumulate(vector3KeyframeTrack->interpolate(fmod(t, action->clip->duration)), weight);
                } else {
                    NOTREACHED() << "Unhandled property mixer";
                    exit(EXIT_FAILURE);
                }
            }
        }

        for (auto const &propertyMixer : propertyMixers) {
            if (auto objectPropertyMixer = dynamic_cast<ObjectPropertyMixer *>(propertyMixer)) {
                objectPropertyMixer->apply();
            }
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
                if (!mixers->translationPropertyMixer) {
                    auto newMixer = new ObjectTranslationPropertyMixer(object);
                    mixers->translationPropertyMixer.reset(newMixer);
                    propertyMixers.insert(newMixer);
                }

                return mixers->translationPropertyMixer.get();
            }
            case OBJECT_PROPERTY_TRANSLATION_X:
            case OBJECT_PROPERTY_TRANSLATION_Y:
            case OBJECT_PROPERTY_TRANSLATION_Z: {
                if (!mixers->splitTranslationPropertyMixer) {
                    auto newMixer = new SplitObjectTranslationPropertyMixer(object);
                    mixers->splitTranslationPropertyMixer.reset(newMixer);
                    propertyMixers.insert(newMixer);
                }

                if (property == OBJECT_PROPERTY_TRANSLATION_X)
                    return &(mixers->splitTranslationPropertyMixer->x);
                else if (property == OBJECT_PROPERTY_TRANSLATION_Y)
                    return &(mixers->splitTranslationPropertyMixer->y);
                else if (property == OBJECT_PROPERTY_TRANSLATION_Z)
                    return &(mixers->splitTranslationPropertyMixer->z);
                break;
            }
            case OBJECT_PROPERTY_ROTATION_X:
            case OBJECT_PROPERTY_ROTATION_Y:
            case OBJECT_PROPERTY_ROTATION_Z: {
                if (!mixers->splitRotationPropertyMixer) {
                    auto newMixer = new SplitObjectRotationPropertyMixer(object);
                    mixers->splitRotationPropertyMixer.reset(newMixer);
                    propertyMixers.insert(newMixer);
                }

                if (property == OBJECT_PROPERTY_ROTATION_X)
                    return &(mixers->splitRotationPropertyMixer->x);
                else if (property == OBJECT_PROPERTY_ROTATION_Y)
                    return &(mixers->splitRotationPropertyMixer->y);
                else if (property == OBJECT_PROPERTY_ROTATION_Z)
                    return &(mixers->splitRotationPropertyMixer->z);
                break;
            }
        }

        NOTREACHED();
        exit(EXIT_FAILURE);
    }

    void AnimationMixer::removePropertyMixers(Object *object) {
        auto it = objectPropertyMixers.find(object);
        if (it == objectPropertyMixers.end()) return;

        auto &mixers = it->second;
        if (mixers->translationPropertyMixer) propertyMixers.erase(mixers->translationPropertyMixer.get());
        objectPropertyMixers.erase(it);
    }

}

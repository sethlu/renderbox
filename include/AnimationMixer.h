#ifndef RENDERBOX_ANIMATIONMIXER_H
#define RENDERBOX_ANIMATIONMIXER_H


#include <vector>
#include <unordered_map>
#include <set>

#include "PropertyMixer.h"
#include "AnimationAction.h"


namespace renderbox {

    class AnimationMixer {
    public:

        std::vector<std::unique_ptr<AnimationAction>> actions;

        void update(float t);

        PropertyMixer *getPropertyMixer(std::shared_ptr<Object> object, OBJECT_PROPERTY property);

        void removePropertyMixers(Object *object);

    protected:

        std::set<PropertyMixer *> propertyMixers;

        struct ObjectPropertyMixers {
            std::unique_ptr<TranslationPropertyMixer> translationPropertyMixer;
        };

        std::unordered_map<Object *, std::unique_ptr<ObjectPropertyMixers>> objectPropertyMixers;

    };

}


#endif //RENDERBOX_ANIMATIONMIXER_H

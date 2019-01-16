#include "PropertyMixer.h"

#include <utility>


namespace renderbox {

    void SplitObjectTranslationPropertyMixer::clear() {
        x.clear();
        y.clear();
        z.clear();
    }

    void SplitObjectTranslationPropertyMixer::apply() {
        object->setTranslation(vec3(x.value(), y.value(), z.value()));
    }

    ObjectTranslationPropertyMixer::ObjectTranslationPropertyMixer(std::shared_ptr<Object> object)
            : TypedObjectPropertyMixer<Object::translation_type>(std::move(object)) {

    }

    void ObjectTranslationPropertyMixer::apply() {
        object->setTranslation(_value);
    }

}

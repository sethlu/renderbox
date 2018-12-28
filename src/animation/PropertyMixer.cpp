#include "PropertyMixer.h"

#include <utility>


namespace renderbox {

    PropertyMixer::PropertyMixer(std::shared_ptr<Object> object)
            : object(std::move(object)) {

    }

    PropertyMixer::~PropertyMixer() = default;

    template<typename V>
    TypedPropertyMixer<V>::TypedPropertyMixer(std::shared_ptr<Object> object)
            : PropertyMixer(object) {

    }

    template<typename V>
    void TypedPropertyMixer<V>::clear() {
        value = {};
    }

    template<typename V>
    void TypedPropertyMixer<V>::accumulate(V incoming, float weight) {
        value += incoming * weight;
    }

    TranslationPropertyMixer::TranslationPropertyMixer(std::shared_ptr<Object> object)
            : TypedPropertyMixer<Object::translation_type>(std::move(object)) {

    }

    void TranslationPropertyMixer::apply() {
        object->setTranslation(value);
    }

}

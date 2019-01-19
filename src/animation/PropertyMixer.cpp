#include "PropertyMixer.h"


namespace renderbox {

    void SplitObjectTranslationPropertyMixer::clear() {
        x.clear();
        y.clear();
        z.clear();
    }

    void SplitObjectTranslationPropertyMixer::apply() {
        object->setTranslation(vec3(x.value(), y.value(), z.value()));
    }

    void SplitObjectRotationPropertyMixer::clear() {
        x.clear();
        y.clear();
        z.clear();
    }

    void SplitObjectRotationPropertyMixer::apply() {
        object->clearRotation();
        object->rotate(vec3(1, 0, 0), glm::radians(x.value()));
        object->rotate(vec3(0, 1, 0), glm::radians(y.value()));
        object->rotate(vec3(0, 0, 1), glm::radians(z.value()));
    }

    ObjectTranslationPropertyMixer::ObjectTranslationPropertyMixer(std::shared_ptr<Object> const &object)
            : TypedObjectPropertyMixer<Object::translation_type>(object) {}

    void ObjectTranslationPropertyMixer::apply() {
        object->setTranslation(_value);
    }

}

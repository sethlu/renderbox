#ifndef RENDERBOX_PROPERTYMIXER_H
#define RENDERBOX_PROPERTYMIXER_H


#include "Object.h"


namespace renderbox {

    enum OBJECT_PROPERTY {
        OBJECT_PROPERTY_TRANSLATION
    };

    class PropertyMixer {
    public:

        explicit PropertyMixer(std::shared_ptr<Object> object);

        virtual ~PropertyMixer();

        std::shared_ptr<Object> object;

        virtual void clear() = 0;

        virtual void apply() = 0;

        virtual OBJECT_PROPERTY getObjectProperty() const = 0;

    };

    template<typename V>
    class TypedPropertyMixer : public PropertyMixer {
    public:

        explicit TypedPropertyMixer(std::shared_ptr<Object> object);

        void clear() override;

        virtual void accumulate(V incoming, float weight);

    protected:

        V value;

    };

    class TranslationPropertyMixer : public TypedPropertyMixer<Object::translation_type> {
    public:

        explicit TranslationPropertyMixer(std::shared_ptr<Object> object);

        void apply() override;

        OBJECT_PROPERTY getObjectProperty() const override {
            return OBJECT_PROPERTY_TRANSLATION;
        };

    };

}


#endif //RENDERBOX_PROPERTYMIXER_H

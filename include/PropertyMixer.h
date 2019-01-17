#ifndef RENDERBOX_PROPERTYMIXER_H
#define RENDERBOX_PROPERTYMIXER_H


#include "Object.h"


namespace renderbox {

    class PropertyMixer {
    public:

        virtual ~PropertyMixer() = default;

        virtual void clear() = 0;

    };

    template<typename V>
    class AccumulableTypedPropertyMixer {
    public:

        virtual void accumulate(V incoming, float weight) = 0;

    };

    typedef AccumulableTypedPropertyMixer<float> AccumulableFloatPropertyMixer;
    typedef AccumulableTypedPropertyMixer<vec3> AccumulableVector3PropertyMixer;

    // Typed property mixer

    template<typename V>
    class TypedPropertyMixer : public PropertyMixer, public AccumulableTypedPropertyMixer<V> {
    public:

        void clear() override {
            _value = {};
        }

        void accumulate(V incoming, float weight) override {
            _value += incoming * weight;
        }

        virtual V value() {
            return _value;
        }

    protected:

        V _value;

    };

    typedef TypedPropertyMixer<float> FloatPropertyMixer;

    // Object property mixer

    enum OBJECT_PROPERTY {
        OBJECT_PROPERTY_TRANSLATION,
        OBJECT_PROPERTY_TRANSLATION_X,
        OBJECT_PROPERTY_TRANSLATION_Y,
        OBJECT_PROPERTY_TRANSLATION_Z,
        OBJECT_PROPERTY_ROTATION_X,
        OBJECT_PROPERTY_ROTATION_Y,
        OBJECT_PROPERTY_ROTATION_Z,
    };

    class ObjectPropertyMixer : public PropertyMixer {
    public:

        explicit ObjectPropertyMixer(std::shared_ptr<Object> object) : object(std::move(object)) {};

        virtual void apply() = 0;

    protected:

        std::shared_ptr<Object> object;

    };

    class SplitObjectTranslationPropertyMixer : public ObjectPropertyMixer {
    public:

        explicit SplitObjectTranslationPropertyMixer(std::shared_ptr<Object> const &object) : ObjectPropertyMixer(object) {}

        void clear() override;

        void apply() override;

        FloatPropertyMixer x;
        FloatPropertyMixer y;
        FloatPropertyMixer z;

    };

    class SplitObjectRotationPropertyMixer : public ObjectPropertyMixer {
    public:

        explicit SplitObjectRotationPropertyMixer(std::shared_ptr<Object> const &object) : ObjectPropertyMixer(object) {}

        void clear() override;

        void apply() override;

        FloatPropertyMixer x;
        FloatPropertyMixer y;
        FloatPropertyMixer z;

    };

    // Typed object property mixer

    template<typename V>
    class TypedObjectPropertyMixer : public ObjectPropertyMixer, public AccumulableTypedPropertyMixer<V> {
    public:

        explicit TypedObjectPropertyMixer(std::shared_ptr<Object> object) : ObjectPropertyMixer(std::move(object)) {}

        void clear() override {
            _value = {};
        }

        void accumulate(V incoming, float weight) override {
            _value += incoming * weight;
        }

    protected:

        V _value;

    };

    class ObjectTranslationPropertyMixer : public TypedObjectPropertyMixer<Object::translation_type> {
    public:

        explicit ObjectTranslationPropertyMixer(std::shared_ptr<Object> const &object);

        void apply() override;

    };

}


#endif //RENDERBOX_PROPERTYMIXER_H

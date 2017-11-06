#ifndef VOX_OBJECT_H
#define VOX_OBJECT_H


#include <vector>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include "Geometry.h"
#include "Material.h"

namespace renderbox {

    class Object {
    protected:

        /**
         * @var Object geometry
         */
        Geometry *geometry = nullptr;

        /**
         * @var Object material
         */
        Material *material = nullptr;

        // Object internal management

        /**
         * @var Global object count
         * @static
         */
        static int count;

        /**
         * @var Global unique object ID
         */
        int objectId = ++count;

        // Object inheritance

        /**
         * @var Parent of the object
         */
        Object *parent = nullptr;

        /**
         * @var Children of the object
         */
        std::vector<Object *> children;

        // Object transformation

        /**
         * @var Object's world matrix
         */
        glm::mat4x4 worldMatrix = glm::mat4x4(1.0f);

        /**
         * @var Object translation
         */
        glm::vec3 translation;

        /**
         * @var Object rotation matrix
         */
        glm::mat4x4 rotationMatrix = glm::mat4x4(1.0f);

        /**
         * @var Object scaling
         */
        glm::vec3 scale = glm::vec3(1.0f);

        virtual void didTransform();

    public:

        Object();

        Object(Geometry *geometry, Material *material);

        int getObjectID() const;

        // Object inheritance

        bool hasParent() const;

        Object *getParent() const;

        std::vector<Object *> getChildren() const;

        void addChild(Object *object);

        // Geometry

        bool hasGeometry();

        Geometry *getGeometry() const;

        // Material

        bool hasMaterial();

        Material *getMaterial() const;

        // Object transformation

        glm::mat4x4 getWorldMatrix() const;

        glm::mat4x4 getMatrix() const;

        glm::vec3 getWorldPosition() const;

        // Translation

        glm::vec3 getTranslation() const;

        void setTranslation(glm::vec3 translation);

        void translate(glm::vec3 delta);

        // Rotation

        glm::mat4x4 getRotationMatrix() const;

        void rotate(glm::vec3 vector, float angle);

        void clearRotation();

        // Scale

        glm::vec3 getScale() const;

        void setScale(glm::vec3 scale);

        // Visibility

        bool visible = true;

    };

}


#endif //VOX_OBJECT_H

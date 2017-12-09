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
        std::shared_ptr<Geometry> geometry;

        /**
         * @var Object material
         */
        std::shared_ptr<Material> material;

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
        std::vector<std::shared_ptr<Object>> children;

        // Object transformation

        /**
         * @var Object's world matrix
         */
        glm::mat4x4 worldMatrix = glm::mat4x4(1.0f);

        /**
         * @var Object translation
         */
        glm::vec3 translation = glm::vec3(0);

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

        Object() = default;

        Object(std::shared_ptr<Geometry> geometry, std::shared_ptr<Material> material);

        virtual ~Object();

        int getObjectId() const;

        // Object inheritance

        bool hasParent() const;

        Object *getParent() const;

        std::vector<std::shared_ptr<Object>> getChildren() const;

        void addChild(std::shared_ptr<Object> child);

        // Geometry

        bool hasGeometry();

        std::shared_ptr<Geometry> getGeometry() const;

        // Material

        bool hasMaterial();

        std::shared_ptr<Material> getMaterial() const;

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

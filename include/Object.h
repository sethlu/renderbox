#ifndef VOX_OBJECT_H
#define VOX_OBJECT_H


#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <vector>
#include "Geometry.h"
#include "Material.h"

namespace renderbox {

    class Object {
	
    protected:

        // Object internal management

        /**
         * @var Global object count
         * @static
         */
        static int count;

        /**
         * @var Global unique object ID
         */
        int objectID = ++count;

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

        virtual void didTransform();

        // Object description

        /**
         * @var Object geometry
         */
        Geometry *geometry = nullptr;

        /**
         * @var Object material
         */
        Material *material = nullptr;

    public:

        Object();
        Object(Geometry *geometry, Material *material);
        int getObjectID();

        bool hasParent();
        Object *getParent();
        std::vector<Object *> getChildren();
        void addChild(Object *object);

        glm::mat4x4 getWorldMatrix();
        glm::mat4x4 getMatrix();
        glm::vec3 getWorldPosition();

        glm::vec3 getTranslation();
        void setTranslation(glm::vec3 translation);
        void translate(glm::vec3 delta);

        glm::mat4x4 getRotationMatrix();
        void rotate(glm::vec3 vector, float angle);
        void clearRotation();

        bool hasGeometry();
        Geometry *getGeometry();

        bool hasMaterial();
        Material *getMaterial();

        bool visible = true;

    };

}


#endif //VOX_OBJECT_H

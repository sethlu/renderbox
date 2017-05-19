#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include "../math/Vector.h"
#include "Object.h"


namespace renderbox {

    int Object::count = 0;

    Object::Object() {

    }

    Object::Object(Geometry *geometry, Material *material) {

        this->geometry = geometry;
        this->material = material;

    }

    int Object::getObjectID() {
        return objectID;
    }

    bool Object::hasParent() {
        return parent != nullptr;
    }

    Object *Object::getParent() {
        return parent;
    }

    std::vector<Object *> Object::getChildren() {
        return children;
    }

    void Object::addChild(Object *child) {
        if (child->hasParent()) {
            fprintf(stderr, "Object already assigned with parent");
            throw 2;
        }
        child->parent = this;
        child->didTransform();
        children.push_back(child);
    }

    glm::mat4x4 Object::getWorldMatrix() {
        return worldMatrix;
    }

    glm::mat4x4 Object::getMatrix() {
        return glm::translate(glm::mat4x4(1.0f), translation) * rotationMatrix;
    }

    glm::vec3 Object::getWorldPosition() {
        return dehomogenize(glm::column(getWorldMatrix(), 3));
    }

    void Object::didTransform() {
        // Calculate world matrix for each child
        worldMatrix = (hasParent() ? getParent()->worldMatrix : glm::mat4x4(1.0f)) * getMatrix();
        // Propagate object transformation
        for (Object *child : children) {
            child->didTransform();
        }
    }

    glm::vec3 Object::getTranslation() {
        return translation;
    }

    void Object::setTranslation(glm::vec3 translation) {
        this->translation = translation;
        didTransform();
    }

    void Object::translate(glm::vec3 delta) {
        translation += delta;
        didTransform();
    }

    glm::mat4x4 Object::getRotationMatrix() {
        return rotationMatrix;
    }

    void Object::rotate(glm::vec3 vector, float angle) {
        rotationMatrix = glm::rotate(glm::mat4x4(1.0f), angle, vector) * rotationMatrix;
        didTransform();
    }

    void Object::clearRotation() {
        rotationMatrix = glm::mat4x4(1.0f);
        didTransform();
    }

    bool Object::hasGeometry() {
        return geometry != nullptr;
    }

    Geometry *Object::getGeometry() {
        return geometry;
    }

    Material *Object::getMaterial() {
        return material;
    }

    bool Object::hasMaterial() {
        return material != nullptr;
    }

}
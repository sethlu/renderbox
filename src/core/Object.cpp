#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include "Vector.h"
#include "Object.h"


namespace renderbox {

    int Object::count = 0;

    Object::Object() = default;

    Object::Object(Geometry *geometry, Material *material) {

        this->geometry = geometry;
        this->material = material;

    }

    int Object::getObjectId() const {
        return objectId;
    }

    bool Object::hasParent() const {
        return parent != nullptr;
    }

    Object *Object::getParent() const {
        return parent;
    };

    std::vector<Object *> Object::getChildren() const {
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

    bool Object::hasGeometry() {
        return geometry != nullptr;
    }

    Geometry *Object::getGeometry() const {
        return geometry;
    }

    bool Object::hasMaterial() {
        return material != nullptr;
    }

    Material *Object::getMaterial() const {
        return material;
    }

    glm::mat4x4 Object::getWorldMatrix() const {
        return worldMatrix;
    }

    glm::mat4x4 Object::getMatrix() const {
        return glm::translate(glm::mat4x4(1.0f), translation) * glm::scale(rotationMatrix, scale);
    }

    glm::vec3 Object::getWorldPosition() const {
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

    glm::vec3 Object::getTranslation() const {
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

    glm::mat4x4 Object::getRotationMatrix() const {
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

    glm::vec3 Object::getScale() const {
        return scale;
    }

    void Object::setScale(glm::vec3 scale) {
        this->scale = scale;
        didTransform();
    }

}
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include "Vector.h"
#include "Object.h"


namespace renderbox {

    int Object::count = 0;

    Object::Object(std::shared_ptr<Geometry> geometry, std::shared_ptr<Material> material)
        : geometry(geometry), material(material) {

    }

    Object::~Object() {
        // Release parents
        for (const auto &child : children) {
            child->parent = nullptr;
        }
    }

    int Object::getObjectId() const {
        return objectId;
    }

    std::shared_ptr<Object> Object::clone() {
        auto o = std::make_shared<Object>(geometry, material);
        o->translation = translation;
        o->rotationMatrix = rotationMatrix;
        o->scale = scale;
        return o;
    }

    bool Object::hasParent() const {
        return parent != nullptr;
    }

    void Object::addChild(std::shared_ptr<Object> child) {
        if (child->hasParent()) {
            fprintf(stderr, "Object already assigned with parent");
            throw 2;
        }

        child->parent = this;
        child->didTransform();

        children.push_back(child);
    }

    bool Object::hasGeometry() {
        return !!geometry;
    }

    std::shared_ptr<Geometry> Object::getGeometry() {
        return geometry;
    }

    void Object::setGeometry(std::shared_ptr<Geometry> geometry_) {
        geometry = std::move(geometry_);
    }

    bool Object::hasMaterial() {
        return !!material;
    }

    std::shared_ptr<Material> Object::getMaterial() {
        return material;
    }

    void Object::setMaterial(std::shared_ptr<Material> material_) {
        material = std::move(material_);
    }

    mat4 Object::getWorldMatrix() const {
        return worldMatrix;
    }

    mat4 Object::getMatrix() const {
        return glm::translate(mat4(1.0f), translation) * glm::scale(rotationMatrix, scale);
    }

    vec3 Object::getWorldPosition() const {
        return dehomogenize(glm::column(getWorldMatrix(), 3));
    }

    void Object::didTransform() {
        // Calculate world matrix for each child
        worldMatrix = (hasParent() ? parent->worldMatrix : mat4(1.0f)) * getMatrix();
        // Propagate object transformation
        for (const auto &child : children) {
            child->didTransform();
        }
        didUpdate();
    }

    vec3 Object::getTranslation() const {
        return translation;
    }

    void Object::setTranslation(vec3 translation) {
        this->translation = translation;
        didTransform();
    }

    void Object::translate(vec3 delta) {
        translation += delta;
        didTransform();
    }

    mat4 Object::getRotationMatrix() const {
        return rotationMatrix;
    }

    void Object::rotate(vec3 vector, float angle) {
        rotationMatrix = glm::rotate(mat4(1.0f), angle, vector) * rotationMatrix;
        didTransform();
    }

    void Object::clearRotation() {
        rotationMatrix = mat4(1.0f);
        didTransform();
    }

    vec3 Object::getScale() const {
        return scale;
    }

    void Object::setScale(vec3 scale) {
        this->scale = scale;
        didTransform();
    }

}

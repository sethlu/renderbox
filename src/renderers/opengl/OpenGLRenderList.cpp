#include <iostream>
#include "OpenGLRenderList.h"


namespace renderbox {

    void OpenGLRenderList::addObject(GLuint programID, Object *object) {
        auto result = objects.find(programID);
        if (result != objects.end()) {
            result->second.insert(result->second.end(), object);
        } else {
            std::vector<Object *> objectList;
            objectList.insert(objectList.end(), object);
            objects.insert(std::pair<GLuint, std::vector<Object *>>(programID, objectList));
        }
    }

}
#include <iostream>
#include "OpenGLRendererProperties.h"


namespace renderbox {

    OpenGLObjectProperties *OpenGLRendererProperties::getObjectProperties(Object *object) {
        std::map<int, OpenGLObjectProperties *>::iterator result = objectProperties.find(object->getObjectID());
        if (result != objectProperties.end()) {
            return result->second;
        }
        OpenGLObjectProperties *properties = new OpenGLObjectProperties();
        objectProperties.insert(std::pair<int, OpenGLObjectProperties *>(object->getObjectID(), properties));
        return properties;
    }

}
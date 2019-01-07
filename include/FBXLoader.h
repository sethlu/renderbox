#ifndef RENDERBOX_FBXLOADER_H
#define RENDERBOX_FBXLOADER_H


#include "Object.h"


namespace renderbox {

    class FBXLoader {
    public:

        explicit FBXLoader(std::shared_ptr<Object> destination);

        void enterFBXSourceFile(char const *filename);

    private:

        std::shared_ptr<Object> destination;

        void enterFBXSource(std::istream &source);

    };

}


#endif //RENDERBOX_FBXLOADER_H

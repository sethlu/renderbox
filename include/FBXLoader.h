#ifndef RENDERBOX_FBXLOADER_H
#define RENDERBOX_FBXLOADER_H


#include "Object.h"


namespace renderbox {

    union FBXPropertyValue {
#define PARAM(TYPE, CTYPE) CTYPE TYPE;
#include "FBXParameterTypes.def"
        void *ptr; // For array & special types
    };

    struct FBXProperty {
        char type;
        FBXPropertyValue value;

        ~FBXProperty() {
            // Free non-primitive-typed value
            switch (type) {
                default:
                if (value.ptr) free(value.ptr);
                break;
#define PARAM(TYPE, CTYPE) case (#TYPE)[0]:
#include "FBXParameterTypes.def"
                break;
            }
        };
    };

    struct FBXNode {
        std::string name;
        std::vector<std::unique_ptr<FBXProperty>> properties;
        std::vector<std::unique_ptr<FBXNode>> subNodes;
    };

    class FBXLoader {
    public:

        explicit FBXLoader(std::shared_ptr<Object> destination);

        void enterFBXSourceFile(char const *filename);

    private:

        std::shared_ptr<Object> destination;

        void enterFBXSource(std::istream &source);

        void handleNode(FBXNode *node);

    };

}


#endif //RENDERBOX_FBXLOADER_H

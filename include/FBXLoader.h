#ifndef RENDERBOX_FBXLOADER_H
#define RENDERBOX_FBXLOADER_H


#include <unordered_map>

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
        size_t size;

        bool isPrimitiveProperty() {
            switch (type) {
                default: return false;
#define PARAM(TYPE, CTYPE) case (#TYPE)[0]:
#include "FBXParameterTypes.def"
                return true;
            }
        }

        ~FBXProperty() {
            // Free non-primitive-typed value
            if (!isPrimitiveProperty()) {
                if (value.ptr) free(value.ptr);
            }
        };
    };

    struct FBXNode {
        std::string name;
        std::vector<std::unique_ptr<FBXProperty>> properties;
        std::vector<std::unique_ptr<FBXNode>> subNodes;
    };

    struct FBXDocument {
        std::vector<std::unique_ptr<FBXNode>> nodes;
        std::unordered_map<std::string, FBXNode *> namedNodes;
    };

    class FBXLoader {
    public:

        explicit FBXLoader(std::shared_ptr<Object> destination);

        void enterFBXSourceFile(char const *filename);

    private:

        std::shared_ptr<Object> destination;

        void enterFBXSource(std::istream &source);

        void parseDocument(FBXDocument *doc);

    };

}


#endif //RENDERBOX_FBXLOADER_H

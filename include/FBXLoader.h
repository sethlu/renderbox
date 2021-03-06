#ifndef RENDERBOX_FBXLOADER_H
#define RENDERBOX_FBXLOADER_H


#include <unordered_map>

#include "Object.h"
#include "MeshGeometry.h"
#include "AnimationMixer.h"


namespace renderbox {

    union FBXPropertyValue {
#define PARAM(TYPE, CTYPE) CTYPE TYPE;
#include "FBXParameterTypes.def"
        void *ptr; // For array & special types
    };

    struct FBXProperty {
        char type;
        FBXPropertyValue value;
        size_t size; // Length of array of specified type

        ~FBXProperty() {
            // Free non-primitive-typed value
            if (!isPrimitiveProperty()) {
                if (value.ptr) free(value.ptr);
            }
        }

        bool isPrimitiveProperty() {
            switch (type) {
                default: return false;
#define PARAM(TYPE, CTYPE) case (#TYPE)[0]:
#include "FBXParameterTypes.def"
                return true;
            }
        }

        std::string toString() {
            if (type != 'S') return {};
            return {reinterpret_cast<char *>(value.ptr), size};
        }
    };

    struct FBXNode {
        std::string name;
        std::vector<std::unique_ptr<FBXProperty>> properties;
        std::vector<std::unique_ptr<FBXNode>> subNodes;
    };

    struct FBXDocument {
        typedef uint32_t version_type;
        typedef int64_t node_id_type;

        version_type version;
        std::vector<std::unique_ptr<FBXNode>> subNodes;

        // Assistant instance variables used in parsing
        std::unordered_map<std::string, FBXNode *> namedSubNodes;
        std::unordered_map<node_id_type, FBXNode *> nodesById;
        std::unordered_map<FBXNode const *, std::pair<std::vector<std::pair<std::string, FBXNode *>>, std::vector<std::pair<std::string, FBXNode *>>>> connections;

        // Results from parsing
        std::unordered_map<FBXNode const *, std::shared_ptr<MeshGeometry>> geometries;
        std::unordered_map<FBXNode const *, std::shared_ptr<Material>> materials;
        std::unordered_map<FBXNode const *, std::shared_ptr<Object>> objects;
        std::unordered_map<FBXNode const *, std::shared_ptr<AnimationClip>> animationClips;
    };

    class FBXLoader {
    public:

        explicit FBXLoader(std::shared_ptr<Object> destination);

        void enterFBXSourceFile(char const *filename);

        std::vector<std::shared_ptr<AnimationMixer>> getAnimationMixers();

    private:

        std::shared_ptr<Object> destination;

        void enterFBXSource(std::istream &source);

        void parseDocument(FBXDocument &doc);

        std::vector<std::shared_ptr<AnimationMixer>> animationMixers;

    };

}


#endif //RENDERBOX_FBXLOADER_H

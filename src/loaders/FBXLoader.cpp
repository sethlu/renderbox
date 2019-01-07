// References
// - https://code.blender.org/2013/08/fbx-binary-file-format-specification/

#include "FBXLoader.h"

#include <fstream>
#include <vector>

#include "logging.h"


namespace renderbox {

    FBXLoader::FBXLoader(std::shared_ptr<Object> destination)
    : destination(std::move(destination)) {}

    void FBXLoader::enterFBXSourceFile(char const *filename) {
        std::ifstream is(filename, std::ifstream::binary);
        if (is) {
            enterFBXSource(is);
        }
    }

    typedef uint32_t version_type;

    template<typename V>
    V get(std::istream &source) {
        V value;
        source.read(reinterpret_cast<char *>(&value), sizeof(value));
        return value;
    }

    std::string getString(std::istream &source, size_t length) {
        std::string buffer(length, '\0');
        source.read(&buffer[0], buffer.size());
        return buffer;
    }

    template<typename V>
    V *getArray(std::istream &source, size_t length) {
        char *array = reinterpret_cast<char *>(malloc(sizeof(V) * length));
        source.read(array, sizeof(V) * length);
        return reinterpret_cast<V *>(array);
    }

    bool isFBXFormatBinary(std::istream &source) {
        std::string const correct = "Kaydara FBX Binary  ";
        std::string buffer = getString(source, correct.size());
        return buffer == correct;
    }

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

    std::unique_ptr<FBXProperty> parseProperty(std::istream &source, version_type version) {

        auto property = std::make_unique<FBXProperty>();
        property->type = get<char>(source);

        switch (property->type) {
            default: {
                LOG(ERROR) << "Unhandled property type: " << property->type << std::endl;
                exit(EXIT_FAILURE);
            }
#define PARAM(TYPE, CTYPE) \
    case (#TYPE)[0]: property->value.TYPE = get<CTYPE>(source); break;
#define ARRAY_PARAM(TYPE, CTYPE) \
    case (#TYPE)[0]: { \
        auto arrayLength = get<uint32_t>(source); \
        auto encoding = get<uint32_t>(source); \
        auto compressedLength = get<uint32_t>(source); \
        if (encoding == 1) { \
            LOG(WARNING) << "Encoding not supported" << std::endl; \
            getArray<char>(source, compressedLength); \
        } else { \
            property->value.ptr = reinterpret_cast<void *>(getArray<CTYPE>(source, arrayLength)); \
        } \
        break; \
    }
#define SPECIAL_PARAM(TYPE) \
    case (#TYPE)[0]: { \
        auto length = get<uint32_t>(source); \
        property->value.ptr = reinterpret_cast<void *>(getArray<char>(source, length)); \
        break; \
    }
#include "FBXParameterTypes.def"
        }

        return property;

    }

    std::unique_ptr<FBXNode> parseNode(std::istream &source, version_type version) {

        // Get end offset
        // Distance from the beginning of the file to the end of the node record
        auto endOffset = version >= 7500 ? get<uint64_t>(source) : get<uint32_t>(source);

        // Get num properties
        // Number of properties in the value typle associated with the node
        auto numProperties = version >= 7500 ? get<uint64_t>(source) : get<uint32_t>(source);

        // Get length of property list (unused)
        version >= 7500 ? get<uint64_t>(source) : get<uint32_t>(source);

        // Get name
        auto nameLen = get<uint8_t>(source);
        auto name = getString(source, nameLen);

        if (endOffset == 0) {
            // NULL terminated record
            return {};
        }

        auto node = std::make_unique<FBXNode>();
        node->name = name;

        for (auto i = 0; i < numProperties; i++) {
            node->properties.emplace_back(parseProperty(source, version));
        }

        while (source.tellg() < endOffset) {
            auto subNode = parseNode(source, version);
            node->subNodes.emplace_back(std::move(subNode));
        }

        return node;

    }

    void FBXLoader::enterFBXSource(std::istream &source) {
        if (!isFBXFormatBinary(source)) {
            NOTREACHED() << "Failed to handle non-binary FBX";
            exit(EXIT_FAILURE);
        }

        // Version is stored in little endian byte order
        auto version = get<version_type>(source.seekg(23));
        LOG(INFO) << "FBX binary version: " << version << std::endl;

        while (auto node = parseNode(source, version)) {
            LOG(INFO) << "Parsed node" << std::endl;
        }

    }

}

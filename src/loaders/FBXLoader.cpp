// References
// - https://code.blender.org/2013/08/fbx-binary-file-format-specification/

#include "FBXLoader.h"

#include <fstream>
#include <vector>

#include <zlib.h>

#include "Mesh.h"
#include "Bone.h"
#include "MeshLambertMaterial.h"
#include "logging.h"


namespace renderbox {

    namespace {

        void debugPrintProperty(FBXProperty const *property) {
            std::cout << " property[" << property->type << "]='";

            switch (property->type) {
                default: break;
#define PARAM(TYPE, CTYPE) \
case (#TYPE)[0]: std::cout << property->value.TYPE; break;
#define ARRAY_PARAM(TYPE, CTYPE) \
case (#TYPE)[0]: \
std::cout << property->size << "["; \
for (int i = 0, n = property->size <= 100 ? property->size : 100; i < n; i++) \
std::cout << reinterpret_cast<CTYPE *>(property->value.ptr)[i] << ", "; \
break;
#include "../../include/FBXParameterTypes.def"
                case 'S': for (auto i = 0; i < property->size; i++) std::cout << reinterpret_cast<char *>(property->value.ptr)[i]; break;
            }

            std::cout << "'";
        }

        void debugPrintNodeBegin(FBXNode const *node, std::string prefix = "") {
            std::cout << prefix << "<" << node->name;
            for (auto &property : node->properties) {
                debugPrintProperty(property.get());
            }
            std::cout << ">";
        }

        void debugPrintNodeEnd(FBXNode const *node, std::string prefix = "") {
            std::cout << prefix << "</" << node->name << ">";
        }

        void debugPrintNode(FBXNode const *node, bool recursive = false, std::string prefix = "") {
            debugPrintNodeBegin(node, prefix);

            if (node->subNodes.size() > 0) {
                std::cout << std::endl;
                if (recursive) {
                    for (auto &subNode : node->subNodes) {
                        debugPrintNode(subNode.get(), recursive, prefix + "  ");
                    }
                } else {
                    std::cout << prefix << "..." << std::endl;
                }
                std::cout << prefix;
            }

            debugPrintNodeEnd(node);
            std::cout << std::endl;
        }

        void debugPrintDocument(FBXDocument const &doc) {
            for (auto const &node : doc.subNodes) {
                debugPrintNode(node.get(), true);
            }
        }

        void debugPrintConnections(FBXDocument const &doc, FBXNode const *node, std::string prefix = "") {
            debugPrintNodeBegin(node, prefix);

            auto const &relationships = doc.connections.at(node);
            auto const &childRelationships = relationships.second;
            if (!childRelationships.empty()) {
                std::cout << std::endl;
                for (auto const &relationship : childRelationships) {
                    if (!relationship.first.empty()) {
                        std::cout << prefix << "  (" + relationship.first + ")" << std::endl;
                    }
                    debugPrintConnections(doc, relationship.second, prefix + "  ");
                }
                std::cout << prefix;
            }

            debugPrintNodeEnd(node);
            std::cout << std::endl;
        }

        void debugPrintConnections(FBXDocument const &doc, FBXDocument::node_id_type id = 0, std::string prefix = "") {
            auto it = doc.nodesById.find(id);
            if (it == doc.nodesById.end()) return;

            debugPrintConnections(doc, it->second, prefix);
        }

    }

    FBXLoader::FBXLoader(std::shared_ptr<Object> destination)
    : destination(std::move(destination)) {}

    void FBXLoader::enterFBXSourceFile(char const *filename) {
        std::ifstream is(filename, std::ifstream::binary);
        if (is) {
            enterFBXSource(is);
        }
    }

    std::vector<std::shared_ptr<AnimationMixer>> FBXLoader::getAnimationMixers() {
        return animationMixers;
    }

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

    template<typename V>
    V *getCompressedArray(std::istream &source, size_t length, size_t compressedLength) {
        // Input
        std::vector<char> compressed(compressedLength);
        source.read(compressed.data(), compressedLength);
        // Output
        size_t arrayByteSize = sizeof(V) * length;
        char *array = reinterpret_cast<char *>(malloc(arrayByteSize));

        z_stream infstream;
        infstream.zalloc = Z_NULL;
        infstream.zfree = Z_NULL;
        infstream.opaque = Z_NULL;
        infstream.avail_in = static_cast<uint>(compressedLength);
        infstream.next_in = reinterpret_cast<Bytef *>(compressed.data());
        infstream.avail_out = static_cast<uint>(arrayByteSize);
        infstream.next_out = reinterpret_cast<Bytef *>(array);

        // Inflate
        inflateInit(&infstream);
        inflate(&infstream, Z_NO_FLUSH);
        inflateEnd(&infstream);

        return reinterpret_cast<V *>(array);
    }

    bool isFBXFormatBinary(std::istream &source) {
        std::string const correct = "Kaydara FBX Binary  ";
        std::string buffer = getString(source, correct.size());
        return buffer == correct;
    }

    std::unique_ptr<FBXProperty> readProperty(std::istream &source, FBXDocument &doc) {
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
            property->value.ptr = reinterpret_cast<void *>(getCompressedArray<CTYPE>(source, arrayLength, compressedLength)); \
        } else { \
            property->value.ptr = reinterpret_cast<void *>(getArray<CTYPE>(source, arrayLength)); \
        } \
        property->size = arrayLength; \
        break; \
    }
#define SPECIAL_PARAM(TYPE) \
    case (#TYPE)[0]: { \
        auto length = get<uint32_t>(source); \
        property->value.ptr = reinterpret_cast<void *>(getArray<char>(source, length)); \
        property->size = length; \
        break; \
    }
#include "../../include/FBXParameterTypes.def"
        }

        return property;

    }

    std::unique_ptr<FBXNode> readNode(std::istream &source, FBXDocument &doc) {
        auto const &version = doc.version;

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
            node->properties.emplace_back(readProperty(source, doc));
        }

        while (source.tellg() < endOffset) {
            if (auto subNode = readNode(source, doc)) {
                node->subNodes.emplace_back(std::move(subNode));
            }
        }

        // Record node by id
        if (node->properties.size() >= 1 && node->properties[0]->type == 'L') {
            doc.nodesById.insert(std::make_pair(node->properties[0]->value.L, node.get()));
        }

        return node;

    }

    FBXDocument readBinaryDocument(std::istream &source) {
        FBXDocument doc;

        // Version is stored in little endian byte order
        doc.version = get<FBXDocument::version_type>(source.seekg(23));
        LOG(INFO) << "FBX binary version: " << doc.version << std::endl;

        while (auto node = readNode(source, doc)) {
            if (!node->name.empty()) {
                doc.namedSubNodes.insert(std::make_pair(node->name, node.get()));
            }
            doc.subNodes.emplace_back(std::move(node));
        }

        return doc;
    }

    void FBXLoader::enterFBXSource(std::istream &source) {
        if (!isFBXFormatBinary(source)) {
            NOTREACHED() << "Failed to handle non-binary FBX";
            exit(EXIT_FAILURE);
        }

        FBXDocument doc = readBinaryDocument(source);
//        debugPrintDocument(doc);
        parseDocument(doc);
    }

    namespace {

        int64_t getNodeId(FBXNode const *node) {
            if (node->properties.size() >= 1 && node->properties[0]->type == 'L') {
                return node->properties[0]->value.L;
            }
            return 0;
        }

        std::string getNodeAttrName(FBXNode const *node) {
            if (node->properties.size() >= 2 && node->properties[1]->type == 'S') {
                return node->properties[1]->toString();
            }
            return "";
        }

        std::string getNodeAttrType(FBXNode const *node) {
            if (node->properties.size() >= 3 && node->properties[2]->type == 'S') {
                return node->properties[2]->toString();
            }
            return "";
        }

        void parseConnections(FBXDocument &doc) {
            auto it = doc.namedSubNodes.find("Connections");
            if (it == doc.namedSubNodes.end()) return;

            auto const &connectionsNode = it->second;
            for (auto const &connectionNode : connectionsNode->subNodes) {

                // Verify connection format

                auto numProperties = connectionNode->properties.size();

                auto safeCheck =
                    numProperties >= 3 &&
                    connectionNode->properties[1]->type == 'L' &&
                    connectionNode->properties[2]->type == 'L';
                if (!safeCheck) {
                    LOG(WARNING) << "Bad connection format" << std::endl;
                    continue;
                }

                auto fromId = connectionNode->properties[1]->value.L;
                auto toId = connectionNode->properties[2]->value.L;
                std::string relationship;

                if (connectionNode->properties.size() >= 4) {
                    safeCheck = connectionNode->properties[3]->type == 'S';
                    if (!safeCheck) {
                        LOG(WARNING) << "Bad connection format" << std::endl;
                        continue;
                    }

                    relationship = connectionNode->properties[3]->toString();
                }

                // Verify connection nodes

                auto fromNodeIt = doc.nodesById.find(fromId);
                auto toNodeIt = doc.nodesById.find(toId);

                safeCheck =
                    fromNodeIt != doc.nodesById.end() &&
                    toNodeIt != doc.nodesById.end();
                if (!safeCheck) {
                    LOG(WARNING) << "Bad connection format" << std::endl;
                    continue;
                }

                // Record connection

                auto fromNode = fromNodeIt->second;
                auto toNode = toNodeIt->second;

                auto fromIt = doc.connections.find(fromNode);
                if (fromIt == doc.connections.end()) {
                    doc.connections.insert(std::pair<FBXNode *, std::pair<std::vector<std::pair<std::string, FBXNode *>>, std::vector<std::pair<std::string, FBXNode *>>>>(fromNode, {{{relationship, toNode}}, {}}));
                } else {
                    fromIt->second.first.emplace_back(std::make_pair(relationship, toNode));
                }

                auto toIt = doc.connections.find(toNode);
                if (toIt == doc.connections.end()) {
                    doc.connections.insert(std::pair<FBXNode *, std::pair<std::vector<std::pair<std::string, FBXNode *>>, std::vector<std::pair<std::string, FBXNode *>>>>(toNode, {{}, {{relationship, fromNode}}}));
                } else {
                    toIt->second.second.emplace_back(std::make_pair(relationship, fromNode));
                }

            }
        }

        void parseMeshGeometrySkinClusterDeformer(FBXDocument &doc, FBXNode const *node, MeshGeometry *geometry, unsigned int clusterIndex) {
            FBXNode const *indexesSubNode = nullptr;
            FBXNode const *weightsSubNode = nullptr;

            for (auto const &subNode : node->subNodes) {
                if (subNode->name == "Indexes") {
                    auto safeCheck =
                        subNode->properties.size() >= 1 &&
                        subNode->properties[0]->type == 'i';
                    if (!safeCheck) {
                        LOG(WARNING) << "Bad indexes format" << std::endl;
                        continue;
                    }
                    indexesSubNode = subNode.get();
                } else if (subNode->name == "Weights") {
                    auto safeCheck =
                        subNode->properties.size() >= 1 &&
                        subNode->properties[0]->type == 'd';
                    if (!safeCheck) {
                        LOG(WARNING) << "Bad weights format" << std::endl;
                        continue;
                    }
                    weightsSubNode = subNode.get();
                }
            }

            if (indexesSubNode && weightsSubNode) {
                auto indexesSize = indexesSubNode->properties[0]->size;
                auto weightsSize = weightsSubNode->properties[0]->size;

                if (indexesSize != weightsSize) {
                    LOG(WARNING) << "Skin cluster indices & weights size mismatch" << std::endl;
                } else {
                    auto indexesArray = reinterpret_cast<int32_t *>(indexesSubNode->properties[0]->value.ptr);
                    auto weightsArray = reinterpret_cast<double_t *>(weightsSubNode->properties[0]->value.ptr);

                    for (auto i = 0; i < indexesSize; i += 1) {
                        auto index = indexesArray[i];

                        if (geometry->skinWeights[index][0] == 0) {
                            geometry->skinIndices[index][0] = clusterIndex;
                            geometry->skinWeights[index][0] = weightsArray[i];
                        } else if (geometry->skinWeights[index][1] == 0) {
                            geometry->skinIndices[index][1] = clusterIndex;
                            geometry->skinWeights[index][1] = weightsArray[i];
                        } else if (geometry->skinWeights[index][2] == 0) {
                            geometry->skinIndices[index][2] = clusterIndex;
                            geometry->skinWeights[index][2] = weightsArray[i];
                        } else if (geometry->skinWeights[index][3] == 0) {
                            geometry->skinIndices[index][3] = clusterIndex;
                            geometry->skinWeights[index][3] = weightsArray[i];
                        }
                    }
                }
            }

        }

        void parseMeshGeometrySkinDeformer(FBXDocument &doc, FBXNode const *node, MeshGeometry *geometry) {
            auto const &relationships = doc.connections.at(node);
            unsigned int clusterIndex = 0;
            for (auto relationship : relationships.second) {
                auto const &childNode = relationship.second;

                auto const &type = getNodeAttrType(childNode);
                if (childNode->name == "Deformer") {
                    if (type == "Cluster") {
                        geometry->skinIndices.resize(geometry->vertices.size());
                        geometry->skinWeights.resize(geometry->vertices.size());

                        parseMeshGeometrySkinClusterDeformer(doc, childNode, geometry, clusterIndex++);
                    }
                }
            }
        }

        std::shared_ptr<MeshGeometry> parseMeshGeometry(FBXDocument &doc, FBXNode const *node) {
            auto geometry = std::make_shared<MeshGeometry>();

            for (auto const &subNode : node->subNodes) {
                if (subNode->name == "Vertices") {
                    auto safeCheck =
                        subNode->properties.size() >= 1 &&
                        subNode->properties[0]->type == 'd';
                    if (!safeCheck) {
                        LOG(WARNING) << "Bad vertices format" << std::endl;
                        continue;
                    }

                    // TODO: Make this more efficient
                    auto size = subNode->properties[0]->size / 3;
                    auto array = reinterpret_cast<double_t *>(subNode->properties[0]->value.ptr);
                    geometry->vertices.resize(size);
                    for (auto i = 0; i < size; i += 1) {
                        geometry->vertices[i] = vec3(array[3 * i],
                                                     array[3 * i + 1],
                                                     array[3 * i + 2]);
                    }

                } else if (subNode->name == "PolygonVertexIndex") {
                    auto safeCheck =
                        subNode->properties.size() >= 1 &&
                        subNode->properties[0]->type == 'i';
                    if (!safeCheck) {
                        LOG(WARNING) << "Bad polygon vertex index format" << std::endl;
                        continue;
                    }

                    // TODO: Make this more efficient
                    std::vector<uvec3::value_type> indices(3);
                    auto array = reinterpret_cast<int32_t *>(subNode->properties[0]->value.ptr);
                    geometry->faces.resize(subNode->properties[0]->size / 4);
                    for (size_t i = 0, n = subNode->properties[0]->size; i < n; i++) {
                        auto endOfFace = array[i] < 0;
                        auto index = endOfFace ? array[i] ^ -1 : array[i];
                        indices.emplace_back(index);

                        if (endOfFace) {
                            if (indices.size() == 3) {
                                geometry->faces.emplace_back(indices[0], indices[1], indices[2]);
                            } else if (indices.size() == 4) {
                                geometry->faces.emplace_back(indices[0], indices[1], indices[2]);
                                geometry->faces.emplace_back(indices[2], indices[3], indices[0]);
                            } else {
                                LOG(WARNING) << "Only tri & quad faces are supported: " << indices.size() << std::endl;
                            }
                            indices.clear();
                        }
                    }
                }
            }

            auto const &relationships = doc.connections.at(node);
            for (auto relationship : relationships.second) {
                auto const &childNode = relationship.second;

                auto const &type = getNodeAttrType(childNode);
                if (childNode->name == "Deformer") {
                    if (type == "Skin") {
                        parseMeshGeometrySkinDeformer(doc, childNode, geometry.get());
                    }
                }
            }

            if (geometry->normals.empty()) geometry->regenerateNormals();

            return geometry;
        }

        void parseGeometries(FBXDocument &doc) {
            auto it = doc.namedSubNodes.find("Objects");
            if (it == doc.namedSubNodes.end()) return;

            auto const &objectsNode = it->second;
            for (auto const &objectNode : objectsNode->subNodes) {
                if (objectNode->name != "Geometry") continue;

                auto const &type = getNodeAttrType(objectNode.get());
                if (type == "Mesh") {
                    auto geometry = parseMeshGeometry(doc, objectNode.get());
                    doc.geometries.insert(std::make_pair(objectNode.get(), geometry));
                }
            }
        }

        std::shared_ptr<Material> parseMaterial(FBXDocument &doc, FBXNode const *node) {
            auto material = std::make_shared<MeshLambertMaterial>();

            for (auto const &subNode : node->subNodes) {
                if (subNode->name == "Properties70") {
                    for (auto const &propertyNode : subNode->subNodes) {
                        if (propertyNode->name != "P") {
                            LOG(WARNING) << "Unexpected node in Properties70, ignored" << std::endl;
                            continue;
                        }

                        auto safeCheck =
                            propertyNode->properties.size() >= 2 &&
                            propertyNode->properties[0]->type == 'S' &&
                            propertyNode->properties[1]->type == 'S';
                        if (!safeCheck) {
                            LOG(WARNING) << "Bad property node format" << std::endl;
                            continue;
                        }

                        vec3 ambientColor;
                        float ambientFactor = 1.f;
                        vec3 diffuseColor;
                        float diffuseFactor = 1.f;

                        auto const &propertyName = propertyNode->properties[0]->toString();
                        auto const &propertyType = propertyNode->properties[1]->toString();
                        if (propertyType == "Color") {
                            safeCheck =
                                propertyNode->properties.size() >= 7 &&
                                propertyNode->properties[4]->type == 'D' &&
                                propertyNode->properties[5]->type == 'D' &&
                                propertyNode->properties[6]->type == 'D';
                            if (!safeCheck) {
                                LOG(WARNING) << "Bad color property node format" << std::endl;
                                continue;
                            }

                            auto const &color = vec3(propertyNode->properties[4]->value.D,
                                                     propertyNode->properties[5]->value.D,
                                                     propertyNode->properties[6]->value.D);
                            if (propertyName == "AmbientColor") {
                                ambientColor = color;
                            } else if (propertyName == "DiffuseColor") {
                                diffuseColor = color;
                            }
                        } else if (propertyType == "Number") {
                            safeCheck =
                                propertyNode->properties.size() >= 5 &&
                                propertyNode->properties[4]->type == 'D';
                            if (!safeCheck) {
                                LOG(WARNING) << "Bad number property node format" << std::endl;
                                continue;
                            }

                            auto const &value = propertyNode->properties[4]->value.D;
                            if (propertyName == "AmbientFactor") {
                                ambientFactor = value;
                            } else if (propertyName == "DiffuseFactor") {
                                diffuseFactor = value;
                            }
                        }

                        material->setAmbientColor(ambientFactor * ambientColor);
                        material->setDiffuseColor(diffuseFactor * diffuseColor);
                    }
                }
            }

            return material;
        }

        void parseMaterials(FBXDocument &doc) {
            auto it = doc.namedSubNodes.find("Objects");
            if (it == doc.namedSubNodes.end()) return;

            auto const &objectsNode = it->second;
            for (auto const &objectNode : objectsNode->subNodes) {
                if (objectNode->name != "Material") continue;

                auto const &material = parseMaterial(doc, objectNode.get());
                doc.materials.insert(std::make_pair(objectNode.get(), material));
            }
        }

        void parseMeshModelMeshGeometrySkinClusterDeformer(FBXDocument &doc, FBXNode const *node, Mesh *mesh, unsigned int clusterIndex) {
            auto const &relationships = doc.connections.at(node);
            for (auto relationship : relationships.second) {
                auto const &childNode = relationship.second;

                auto const &type = getNodeAttrType(childNode);
                if (childNode->name == "Model" && type == "LimbNode") {
                    auto it = doc.objects.find(childNode);
                    if (it == doc.objects.end()) {
                        LOG(WARNING) << "Unexpected limb model referenced in deformer: limb node id " << getNodeId(childNode) << std::endl;
                        continue;
                    }
                    if (auto bone = std::dynamic_pointer_cast<Bone>(it->second)) {
                        mesh->bones.emplace_back(bone);
                    }
                }
            }
        }

        void parseMeshModelMeshGeometrySkinDeformer(FBXDocument &doc, FBXNode const *node, Mesh *mesh) {
            auto const &relationships = doc.connections.at(node);
            unsigned int clusterIndex = 0;
            for (auto relationship : relationships.second) {
                auto const &childNode = relationship.second;

                auto const &type = getNodeAttrType(childNode);
                if (childNode->name == "Deformer") {
                    if (type == "Cluster") {
                        parseMeshModelMeshGeometrySkinClusterDeformer(doc, childNode, mesh, clusterIndex++);
                    }
                }
            }
        }

        void parseModels(FBXDocument &doc, std::shared_ptr<Object> &dest, FBXNode const *node) {
            auto const &relationships = doc.connections.at(node);
            for (auto const &relationship : relationships.second) {
                auto const &childNode = relationship.second;

                if (childNode->name != "Model") continue;

                std::shared_ptr<Object> object;

                auto const &type = getNodeAttrType(childNode);
                if (type == "Mesh") {
                    object = std::make_shared<Mesh>();
                } else if (type == "LimbNode") {
                    object = std::make_shared<Bone>();
                } else {
                    LOG(WARNING) << "Unhandled model type" << std::endl;
                    continue;
                }

                for (auto const &subNode : childNode->subNodes) {
                    if (subNode->name == "Properties70") {
                        for (auto const &propertyNode : subNode->subNodes) {
                            if (propertyNode->name != "P") {
                                LOG(WARNING) << "Unexpected node in Properties70, ignored" << std::endl;
                                continue;
                            }

                            auto safeCheck =
                                propertyNode->properties.size() >= 2 &&
                                propertyNode->properties[0]->type == 'S' &&
                                propertyNode->properties[1]->type == 'S';
                            if (!safeCheck) {
                                LOG(WARNING) << "Bad property node format" << std::endl;
                                continue;
                            }

                            auto const &propertyName = propertyNode->properties[0]->toString();
                            auto const &propertyType = propertyNode->properties[1]->toString();
                            if (propertyType == "Lcl Translation") {
                                safeCheck =
                                    propertyNode->properties.size() >= 7 &&
                                    propertyNode->properties[4]->type == 'D' &&
                                    propertyNode->properties[5]->type == 'D' &&
                                    propertyNode->properties[6]->type == 'D';
                                if (!safeCheck) {
                                    LOG(WARNING) << "Bad color property node format" << std::endl;
                                    continue;
                                }

                                auto const &value = vec3(propertyNode->properties[4]->value.D,
                                                         propertyNode->properties[5]->value.D,
                                                         propertyNode->properties[6]->value.D);
                                if (propertyName == "Lcl Translation") {
                                    object->setTranslation(value);
                                }
                            }
                        }
                    }
                }

                doc.objects.insert(std::make_pair(childNode, object));

                dest->addChild(object);

                parseModels(doc, object, childNode);

                auto const &relationships = doc.connections.at(childNode);
                for (auto const &relationship : relationships.second) {
                    auto const &childNode = relationship.second;

                    if (childNode->name == "Geometry") {
                        auto it = doc.geometries.find(childNode);
                        if (it == doc.geometries.end()) {
                            LOG(WARNING) << "Referenced geometry in mesh model cannot be found" << std::endl;
                            continue;
                        }
                        object->setGeometry(it->second);

                        auto const &relationships = doc.connections.at(childNode);
                        for (auto relationship : relationships.second) {
                            auto const &childNode = relationship.second;

                            auto const &type = getNodeAttrType(childNode);
                            if (childNode->name == "Deformer") {
                                if (type == "Skin") {
                                    if (object->isMesh()) {
                                        if (auto mesh = dynamic_cast<Mesh *>(object.get())) {
                                            parseMeshModelMeshGeometrySkinDeformer(doc, childNode, mesh);
                                        }
                                    }
                                }
                            }
                        }

                    } else if (childNode->name == "Material") {
                        auto it = doc.materials.find(childNode);
                        if (it == doc.materials.end()) {
                            LOG(WARNING) << "Referenced material in mesh model cannot be found" << std::endl;
                            continue;
                        }
                        object->setMaterial(it->second);
                    }
                }
            }
        }

        void parseModels(FBXDocument &doc, std::shared_ptr<Object> &dest) {
            auto it = doc.nodesById.find(0);
            if (it == doc.nodesById.end()) return;

            parseModels(doc, dest, it->second);
        }

    }

    std::shared_ptr<KeyframeTrack> parseAnimationCurve(FBXDocument &doc, FBXNode const *node) {
        FBXNode const *timeSubNode = nullptr;
        FBXNode const *valueSubNode = nullptr;

        for (auto const &subNode : node->subNodes) {
            if (subNode->name == "KeyTime") {
                auto safeCheck =
                    subNode->properties.size() >= 1 &&
                    subNode->properties[0]->type == 'l';
                if (!safeCheck) {
                    LOG(WARNING) << "Bad key time format" << std::endl;
                    continue;
                }
                timeSubNode = subNode.get();
            } else if (subNode->name == "KeyValueFloat") {
                auto safeCheck =
                    subNode->properties.size() >= 1 &&
                    subNode->properties[0]->type == 'f';
                if (!safeCheck) {
                    LOG(WARNING) << "Bad key value format" << std::endl;
                    continue;
                }
                valueSubNode = subNode.get();
            }
        }

        auto keyframeTrack = std::make_shared<FloatKeyframeTrack>();

        if (timeSubNode && valueSubNode) {
            auto timeSize = timeSubNode->properties[0]->size;
            auto valueSize = valueSubNode->properties[0]->size;

            if (timeSize != valueSize) {
                LOG(WARNING) << "Skin cluster indices & weights size mismatch" << std::endl;
            } else {
                auto timeArray = reinterpret_cast<long *>(timeSubNode->properties[0]->value.ptr);
                auto valueArray = reinterpret_cast<float *>(valueSubNode->properties[0]->value.ptr);

                keyframeTrack->times.resize(timeSize);
                keyframeTrack->values.resize(valueSize);

                for (auto i = 0; i < timeSize; i++) {
                    keyframeTrack->times[i] = timeArray[i] / 46186158000.f;
                    keyframeTrack->values[i] = valueArray[i];
                }
            }
        }

        return keyframeTrack;
    }

    void parseAnimationCurveNode(FBXDocument &doc, FBXNode const *node, AnimationMixer *mixer, AnimationAction *action) {
        auto const &name = getNodeAttrName(node);

        auto keyframeTrack = std::make_shared<KeyframeTrack>();

        auto const &relationships = doc.connections.at(node);
        if (name[0] == 'T') { // Not the best way check name
            for (auto const &relationship : relationships.second) {
                auto const &childNode = relationship.second;
                if (relationship.first == "d|X") {
                    auto keyframeTrack = parseAnimationCurve(doc, childNode);
                    auto i = action->clip->tracks.size();
                    action->clip->tracks.emplace_back(keyframeTrack);

                    for (auto const &relationship : relationships.first) {
                        auto const &parentNode = relationship.second;
                        if (parentNode->name != "Model") continue;
                        action->bind(mixer->getPropertyMixer(doc.objects.at(parentNode),
                                                             OBJECT_PROPERTY_TRANSLATION_X), i);
                    }
                } else if (relationship.first == "d|Y") {
                    auto keyframeTrack = parseAnimationCurve(doc, childNode);
                    auto i = action->clip->tracks.size();
                    action->clip->tracks.emplace_back(keyframeTrack);

                    for (auto const &relationship : relationships.first) {
                        auto const &parentNode = relationship.second;
                        if (parentNode->name != "Model") continue;
                        action->bind(mixer->getPropertyMixer(doc.objects.at(parentNode),
                                                             OBJECT_PROPERTY_TRANSLATION_Y), i);
                    }
                } else if (relationship.first == "d|Z") {
                    auto keyframeTrack = parseAnimationCurve(doc, childNode);
                    auto i = action->clip->tracks.size();
                    action->clip->tracks.emplace_back(keyframeTrack);

                    for (auto const &relationship : relationships.first) {
                        auto const &parentNode = relationship.second;
                        if (parentNode->name != "Model") continue;
                        action->bind(mixer->getPropertyMixer(doc.objects.at(parentNode),
                                                             OBJECT_PROPERTY_TRANSLATION_Z), i);
                    }
                }
            }
        } else if (name[0] == 'R') { // Not the best way check name
            for (auto const &relationship : relationships.second) {
                auto const &childNode = relationship.second;
                if (relationship.first == "d|X") {
                    auto keyframeTrack = parseAnimationCurve(doc, childNode);
                    auto i = action->clip->tracks.size();
                    action->clip->tracks.emplace_back(keyframeTrack);

                    for (auto const &relationship : relationships.first) {
                        auto const &parentNode = relationship.second;
                        if (parentNode->name != "Model") continue;
                        action->bind(mixer->getPropertyMixer(doc.objects.at(parentNode),
                                                             OBJECT_PROPERTY_ROTATION_X), i);
                    }
                } else if (relationship.first == "d|Y") {
                    auto keyframeTrack = parseAnimationCurve(doc, childNode);
                    auto i = action->clip->tracks.size();
                    action->clip->tracks.emplace_back(keyframeTrack);

                    for (auto const &relationship : relationships.first) {
                        auto const &parentNode = relationship.second;
                        if (parentNode->name != "Model") continue;
                        action->bind(mixer->getPropertyMixer(doc.objects.at(parentNode),
                                                             OBJECT_PROPERTY_ROTATION_Y), i);
                    }
                } else if (relationship.first == "d|Z") {
                    auto keyframeTrack = parseAnimationCurve(doc, childNode);
                    auto i = action->clip->tracks.size();
                    action->clip->tracks.emplace_back(keyframeTrack);

                    for (auto const &relationship : relationships.first) {
                        auto const &parentNode = relationship.second;
                        if (parentNode->name != "Model") continue;
                        action->bind(mixer->getPropertyMixer(doc.objects.at(parentNode),
                                                             OBJECT_PROPERTY_ROTATION_Z), i);
                    }
                }
            }
        }
    }

    std::shared_ptr<AnimationMixer> parseAnimationStacks(FBXDocument &doc) {
        auto animationMixer = std::make_shared<AnimationMixer>();

        auto it = doc.namedSubNodes.find("Objects");
        if (it == doc.namedSubNodes.end()) return animationMixer;

        auto const &objectsNode = it->second;
        for (auto const &objectNode : objectsNode->subNodes) {
            if (objectNode->name != "AnimationStack") continue;

            auto animationClip = std::make_shared<AnimationClip>();
            doc.animationClips.insert(std::make_pair(objectNode.get(), animationClip));

            auto animationAction = std::make_unique<AnimationAction>();
            animationAction->clip = animationClip;

            for (auto const &subNode : objectNode->subNodes) {
                if (subNode->name == "Properties70") {
                    for (auto const &propertyNode : subNode->subNodes) {
                        if (propertyNode->name != "P") {
                            LOG(WARNING) << "Unexpected node in Properties70, ignored" << std::endl;
                            continue;
                        }

                        auto safeCheck =
                            propertyNode->properties.size() >= 2 &&
                            propertyNode->properties[0]->type == 'S' &&
                            propertyNode->properties[1]->type == 'S';
                        if (!safeCheck) {
                            LOG(WARNING) << "Bad property node format" << std::endl;
                            continue;
                        }

                        auto const &propertyName = propertyNode->properties[0]->toString();
                        auto const &propertyType = propertyNode->properties[1]->toString();
                        if (propertyType == "KTime") {
                            safeCheck =
                                propertyNode->properties.size() >= 5 &&
                                propertyNode->properties[4]->type == 'L';
                            if (!safeCheck) {
                                LOG(WARNING) << "Bad time property node format" << std::endl;
                                continue;
                            }

                            auto const &time = propertyNode->properties[4]->value.L / 46186158000.f;
                            if (propertyName == "LocalStop") {
                                animationClip->duration = time;
                            }
                        }
                    }
                }
            }

            auto const &relationships = doc.connections.at(objectNode.get());
            for (auto const &relationship : relationships.second) {
                auto const &childNode = relationship.second;
                if (childNode->name != "AnimationLayer") continue;

                auto const &relationships = doc.connections.at(childNode);
                for (auto const &relationship : relationships.second) {
                    auto const &childNode = relationship.second;
                    if (childNode->name != "AnimationCurveNode") continue;

                    parseAnimationCurveNode(doc, childNode, animationMixer.get(), animationAction.get());
                }
            }

            animationMixer->actions.emplace_back(std::move(animationAction));
        }

        return animationMixer;
    }

    void FBXLoader::parseDocument(FBXDocument &doc) {
        parseConnections(doc);
//        debugPrintConnections(doc);
        parseGeometries(doc);
        parseMaterials(doc);
        parseModels(doc, destination);
        auto animationMixer = parseAnimationStacks(doc);
        animationMixers.emplace_back(animationMixer);
    }

}

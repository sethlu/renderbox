#ifndef RENDERBOX_OBJLOADER_H
#define RENDERBOX_OBJLOADER_H


#include <ostream>
#include <tuple>
#include <unordered_map>
#include "lexer.h"
#include "Scene.h"

namespace std {
    namespace {

        // Code from boost
        // Reciprocal of the golden ratio helps spread entropy
        //     and handles duplicates.
        // See Mike Seymour in magic-numbers-in-boosthash-combine:
        //     https://stackoverflow.com/questions/4948780

        template <class T>
        inline void hash_combine(std::size_t& seed, T const& v) {
            seed ^= hash<T>()(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
        }

        // Recursive template code derived from Matthieu M.
        template <class Tuple, size_t Index = std::tuple_size<Tuple>::value - 1>
        struct HashValueImpl {
            static void apply(size_t& seed, Tuple const& tuple) {
                HashValueImpl<Tuple, Index-1>::apply(seed, tuple);
                hash_combine(seed, get<Index>(tuple));
            }
        };

        template <class Tuple>
        struct HashValueImpl<Tuple,0> {
            static void apply(size_t& seed, Tuple const& tuple) {
                hash_combine(seed, get<0>(tuple));
            }
        };

    }

    template <typename ... TT>
    struct hash<std::tuple<TT...>> {
        size_t
        operator()(std::tuple<TT...> const& tt) const {
            size_t seed = 0;
            HashValueImpl<std::tuple<TT...> >::apply(seed, tt);
            return seed;
        }
    };
}

namespace renderbox {

    namespace obj_tok {

        enum OBJTokenKind {
#define TOK(X) X,
#include "OBJTokenKinds.def"
            OBJ_NUM_TOKENS
        };

    }

    namespace mtl_tok {

        enum MTLTokenKind {
#define TOK(X) X,
#include "MTLTokenKinds.def"
            MTL_NUM_TOKENS
        };

    }

    struct OBJToken {

        obj_tok::OBJTokenKind kind;
        const char *pointer;
        unsigned len;

        friend std::ostream &operator<<(std::ostream &os, const OBJToken &token);

    };

    struct MTLToken {

        mtl_tok::MTLTokenKind kind;
        const char *pointer;
        unsigned len;

    };

    class OBJLexer;
    class MTLLexer;

    inline vec3 xyz(const vec3& v) { return {v.x, v.y, v.z}; }
    inline vec3 xzy(const vec3& v) { return {v.x, v.z, v.y}; }
    inline vec3 yxz(const vec3& v) { return {v.y, v.x, v.z}; }
    inline vec3 yzx(const vec3& v) { return {v.y, v.z, v.x}; }
    inline vec3 zxy(const vec3& v) { return {v.z, v.x, v.z}; }
    inline vec3 zyx(const vec3& v) { return {v.z, v.z, v.x}; }

    inline vec3 nxyz(const vec3& v) { return {-v.x, v.y, v.z}; }
    inline vec3 nxzy(const vec3& v) { return {-v.x, v.z, v.y}; }
    inline vec3 nyxz(const vec3& v) { return {-v.y, v.x, v.z}; }
    inline vec3 nyzx(const vec3& v) { return {-v.y, v.z, v.x}; }
    inline vec3 nzxy(const vec3& v) { return {-v.z, v.x, v.z}; }
    inline vec3 nzyx(const vec3& v) { return {-v.z, v.z, v.x}; }

    inline vec3 xnyz(const vec3& v) { return {v.x, -v.y, v.z}; }
    inline vec3 xnzy(const vec3& v) { return {v.x, -v.z, v.y}; }
    inline vec3 ynxz(const vec3& v) { return {v.y, -v.x, v.z}; }
    inline vec3 ynzx(const vec3& v) { return {v.y, -v.z, v.x}; }
    inline vec3 znxy(const vec3& v) { return {v.z, -v.x, v.z}; }
    inline vec3 znyx(const vec3& v) { return {v.z, -v.z, v.x}; }

    inline vec3 xynz(const vec3& v) { return {v.x, v.y, -v.z}; }
    inline vec3 xzny(const vec3& v) { return {v.x, v.z, -v.y}; }
    inline vec3 yxnz(const vec3& v) { return {v.y, v.x, -v.z}; }
    inline vec3 yznx(const vec3& v) { return {v.y, v.z, -v.x}; }
    inline vec3 zxny(const vec3& v) { return {v.z, v.x, -v.z}; }
    inline vec3 zynx(const vec3& v) { return {v.z, v.z, -v.x}; }

    inline vec3 nxnyz(const vec3& v) { return {-v.x, -v.y, v.z}; }
    inline vec3 nxnzy(const vec3& v) { return {-v.x, -v.z, v.y}; }
    inline vec3 nynxz(const vec3& v) { return {-v.y, -v.x, v.z}; }
    inline vec3 nynzx(const vec3& v) { return {-v.y, -v.z, v.x}; }
    inline vec3 nznxy(const vec3& v) { return {-v.z, -v.x, v.z}; }
    inline vec3 nznyx(const vec3& v) { return {-v.z, -v.z, v.x}; }

    inline vec3 nxynz(const vec3& v) { return {-v.x, v.y, -v.z}; }
    inline vec3 nxzny(const vec3& v) { return {-v.x, v.z, -v.y}; }
    inline vec3 nyxnz(const vec3& v) { return {-v.y, v.x, -v.z}; }
    inline vec3 nyznx(const vec3& v) { return {-v.y, v.z, -v.x}; }
    inline vec3 nzxny(const vec3& v) { return {-v.z, v.x, -v.z}; }
    inline vec3 nzynx(const vec3& v) { return {-v.z, v.z, -v.x}; }

    inline vec3 xnynz(const vec3& v) { return {v.x, -v.y, -v.z}; }
    inline vec3 xnzny(const vec3& v) { return {v.x, -v.z, -v.y}; }
    inline vec3 ynxnz(const vec3& v) { return {v.y, -v.x, -v.z}; }
    inline vec3 ynznx(const vec3& v) { return {v.y, -v.z, -v.x}; }
    inline vec3 znxny(const vec3& v) { return {v.z, -v.x, -v.z}; }
    inline vec3 znynx(const vec3& v) { return {v.z, -v.z, -v.x}; }

    inline vec3 nxnynz(const vec3& v) { return {-v.x, -v.y, -v.z}; }
    inline vec3 nxnzny(const vec3& v) { return {-v.x, -v.z, -v.y}; }
    inline vec3 nynxnz(const vec3& v) { return {-v.y, -v.x, -v.z}; }
    inline vec3 nynznx(const vec3& v) { return {-v.y, -v.z, -v.x}; }
    inline vec3 nznxny(const vec3& v) { return {-v.z, -v.x, -v.z}; }
    inline vec3 nznynx(const vec3& v) { return {-v.z, -v.z, -v.x}; }

    class OBJLoader {
    public:

        explicit OBJLoader(std::shared_ptr<Object> destination);

        void enterOBJSourceFile(const char *filename);

        void enterOBJSourceFile(const std::string &filename);

        void enterMTLSourceFile(const char *filename);

        vec3 (*vertexNormalTransform)(const vec3 &v) = xyz;

    private:

        std::vector<const char *> sourceFiles;

        std::shared_ptr<Object> destination;

        std::vector<glm::vec3> vertices;

        std::vector<glm::vec2> uvs;

        std::vector<glm::vec3> normals;

        Object *object;

        Geometry *geometry;

        std::unordered_map<std::tuple<int, int, int>, unsigned> geometryVertexIndices;

        Material *material;

        std::unordered_map<std::string, std::shared_ptr<Material>> materials;

        void enterOBJSource(const char *source);

        void enterMTLSource(const char *source);

        void lex(OBJLexer &lexer, OBJToken &token);

        void lex(MTLLexer &lexer, MTLToken &token);

        void handleVertex(OBJLexer &lexer, OBJToken &token);

        void handleTextureVertex(OBJLexer &lexer, OBJToken &token);

        void handleVertexNormal(OBJLexer &lexer, OBJToken &token);

        void handleFace(OBJLexer &lexer, OBJToken &token);

        void handleObject(OBJLexer &lexer, OBJToken &token);

        void handleMaterialLibrary(OBJLexer &lexer, OBJToken &token);

        void handleMaterialName(OBJLexer &lexer, OBJToken &token);

        void handleNewMaterial(MTLLexer &lexer, MTLToken &token);

        void handleMaterialColor(MTLLexer &lexer, MTLToken &token);

        void handleTextureMap(MTLLexer &lexer, MTLToken &token);

        float parseFloat(OBJLexer &lexer, OBJToken &token);

        float parseFloat(MTLLexer &lexer, MTLToken &token);

        int parseInt(OBJLexer &lexer, OBJToken &token);

        unsigned getVertexIndex(int v, int vt, int vn);

    };

    class OBJLexer {

        friend class OBJLoader;

    public:

        explicit OBJLexer(const char *bufferStart, const char *bufferPointer);

        bool lex(OBJToken &token);

    private:

        const char *bufferStart;
        const char *bufferPointer;

        bool isAtPhysicalStartOfLine;
        bool isLexingFilename;
        bool isLexingMaterialName;

        unsigned line;

        bool lexNumericConstant(OBJToken &token, const char *pointer);

        bool lexIdentifier(OBJToken &token, const char *pointer);

        bool lexUnquotedStringLiteral(OBJToken &token, const char *pointer);

        bool skipLineComment(OBJToken &token, const char *pointer);

        bool skipHorizontalWhitespace(OBJToken &token);

    };

    class MTLLexer {

        friend class OBJLoader;

    public:

        explicit MTLLexer(const char *bufferStart, const char *bufferPointer);

        bool lex(MTLToken &token);

    private:

        const char *bufferStart;
        const char *bufferPointer;

        bool isAtPhysicalStartOfLine;
        bool isLexingFilename;
        bool isLexingMaterialName;

        unsigned line;

        bool lexNumericConstant(MTLToken &token, const char *pointer);

        bool lexIdentifier(MTLToken &token, const char *pointer);

        bool lexUnquotedStringLiteral(MTLToken &token, const char *pointer);

        bool skipLineComment(MTLToken &token, const char *pointer);

        bool skipHorizontalWhitespace(MTLToken &token);

    };

}


#endif //RENDERBOX_OBJLOADER_H

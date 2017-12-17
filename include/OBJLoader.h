#ifndef RENDERBOX_OBJLOADER_H
#define RENDERBOX_OBJLOADER_H


#include <ostream>
#include "lexer.h"
#include "Scene.h"

namespace renderbox {

    namespace obj_tok {

        enum OBJTokenKind {
#define TOK(X) X,
#include "OBJTokenKinds.def"
            OBJ_NUM_TOKENS
        };

    }

    struct OBJToken {

        obj_tok::OBJTokenKind kind;
        const char *pointer;
        unsigned len;

        friend std::ostream &operator<<(std::ostream &os, const OBJToken &token);

    };

    class OBJLexer;

    class OBJLoader {
    public:

        explicit OBJLoader(std::shared_ptr<Object> destination);

        void loadOBJ(const char *source);

    private:

        std::shared_ptr<Object> destination;

        std::vector<glm::vec3> vertices;

        std::vector<glm::vec3> normals;

        Object *object;

        Geometry *geometry;

        void lex(OBJLexer &lexer, OBJToken &token);

        void handleVertex(OBJLexer &lexer, OBJToken &token);

        void handleVertexNormal(OBJLexer &lexer, OBJToken &token);

        void handleFace(OBJLexer &lexer, OBJToken &token);

        void handleObject(OBJLexer &lexer, OBJToken &token);

        float parseFloat(OBJLexer &lexer, OBJToken &token);

        int parseInt(OBJLexer &lexer, OBJToken &token);

    };

    class OBJLexer {
    public:

        explicit OBJLexer(const char *bufferStart, const char *bufferPointer);

        bool lex(OBJToken &token);

    private:

        const char *bufferStart;
        const char *bufferPointer;

        bool isAtPhysicalStartOfLine;
        bool isLexingFilename;

        unsigned line;

        bool lexNumericConstant(OBJToken &token, const char *pointer);

        bool lexIdentifier(OBJToken &token, const char *pointer);

        bool lexUnquotedStringLiteral(OBJToken &token, const char *pointer);

        bool skipLineComment(OBJToken &token, const char *pointer);

        bool skipHorizontalWhitespace(OBJToken &token);

    };

    class OBJParser {
    public:



    };

}


#endif //RENDERBOX_OBJLOADER_H

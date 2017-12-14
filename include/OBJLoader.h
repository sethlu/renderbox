#ifndef RENDERBOX_OBJLOADER_H
#define RENDERBOX_OBJLOADER_H


#include <ostream>
#include "lexer.h"

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

    class OBJLoader {
    public:



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

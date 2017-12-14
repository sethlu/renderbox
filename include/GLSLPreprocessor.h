#ifndef RENDERBOX_GLSLPREPROCESSOR_H
#define RENDERBOX_GLSLPREPROCESSOR_H


#include <memory>
#include <vector>
#include <map>
#include <ostream>
#include "lexer.h"

namespace renderbox {

    namespace glsl_tok {

        enum GLSLTokenKind {
#define TOK(X) X,
#include "GLSLTokenKinds.def"
            GLSL_NUM_TOKENS
        };

        enum GLSLPPKeywordKind {
#define PPKEYWORD(X) pp_##X,
#include "GLSLTokenKinds.def"
            GLSL_NUM_PP_KEYWORDS
        };

    }

    struct GLSLToken {

        glsl_tok::GLSLTokenKind kind;
        const char *pointer;
        unsigned len;

        friend std::ostream &operator<<(std::ostream &os, const GLSLToken &token);

    };

    struct CstrComparator {
        bool operator()(const char *a, const char *b) const {
            return strcmp(a, b) == 0;
        }
    };

    class GLSLPreprocessorSource;
    class GLSLPreprocessorLexer;

    class GLSLPreprocessor {

        friend class GLSLPreprocessorLexer;

    public:

        void lex(GLSLToken &token);

        void enterMainSourceFile(const char *filename);

        void enterMainSource(const char *source);

        // Bootstrap

        std::string bootstrap;

    private:

        void enterSourceFile(const char *filename);

        void enterSource(const char *source);

        void enterSource(const std::string &source);

        void exitSource();

        void handleDirective(GLSLToken &token);

        // Sources

        std::map<const char *, std::unique_ptr<GLSLPreprocessorSource>, CstrComparator> sources;

        // Lexers

        GLSLPreprocessorLexer *currentLexer;

        std::vector<std::unique_ptr<GLSLPreprocessorLexer>> lexers;

    };

    std::string preprocessGLSLSource(const char *source, const std::string &bootstrap = "");

    std::string readFile(const char *filename);

    std::string preprocessGLSLSourceFile(const char *filename, const std::string &bootstrap = "");

    struct GLSLPreprocessorSource {

        std::unique_ptr<char> source;
        size_t size;

        explicit GLSLPreprocessorSource(const char *filename);

    };

    class GLSLPreprocessorLexer {

        friend class GLSLPreprocessor;

    public:

        GLSLPreprocessorLexer(GLSLPreprocessor *preprocessor,
                              const char *bufferStart,
                              const char *bufferPointer);

        bool lex(GLSLToken &token);

    private:

        GLSLPreprocessor *preprocessor;

        const char *bufferStart;
        const char *bufferPointer;

        bool isAtPhysicalStartOfLine;
        bool isPreprocessingDirective;
        bool isLexingFilename;

        unsigned line;

        bool lexLine(GLSLToken &token, const char *pointer);

        bool lexIdentifier(GLSLToken &token, const char *pointer);

        bool lexStringLiteral(GLSLToken &token, const char *pointer);

        bool lexAngledStringLiteral(GLSLToken &token, const char *pointer);

        bool skipHorizontalWhitespace(GLSLToken &token);

    };

}


#endif //RENDERBOX_GLSLPREPROCESSOR_H

#include <iostream>
#include <fstream>
#include "GLSLPreprocessor.h"


namespace renderbox {

    std::ostream &operator<<(std::ostream &os, const GLSLToken &token) {

        char cstr[token.len + 1];
        memcpy(cstr, token.pointer, token.len);
        cstr[token.len] = '\0';

        os << cstr;

        return os;

    }

    void GLSLPreprocessor::lex(GLSLToken &token) {

        bool returnedToken;
        do {
            returnedToken = currentLexer->lex(token);

            if (token.kind == glsl_tok::eof) {
                exitSource();

                // Still lexers running?
                if (!lexers.empty()) returnedToken = false; // Force to return a next token
            }

        } while (!returnedToken);

    }

    void GLSLPreprocessor::enterMainSourceFile(const char *filename) {
        enterSourceFile(filename);
    }

    void GLSLPreprocessor::enterMainSource(const char *source) {
        enterSource(source);
    }

    void GLSLPreprocessor::enterSourceFile(const char *filename) {

        Source *source;
        auto it = sources.find(filename);
        if (it == sources.end()) {
            source = new Source(filename);
            sources.insert(std::make_pair(filename, std::unique_ptr<Source>(source)));
        } else {
            source = it->second.get();
        }

        const char *bufferStart = source->source.get();

        currentLexer = new GLSLPreprocessorLexer(this, bufferStart, bufferStart);
        lexers.push_back(std::unique_ptr<GLSLPreprocessorLexer>(currentLexer));

    }

    void GLSLPreprocessor::enterSource(const char *source) {

        currentLexer = new GLSLPreprocessorLexer(this, source, source);
        lexers.push_back(std::unique_ptr<GLSLPreprocessorLexer>(currentLexer));

    }

    void GLSLPreprocessor::enterSource(const std::string &source) {
        enterSource(source.c_str());
    }

    void GLSLPreprocessor::exitSource() {

        lexers.pop_back();
        currentLexer = lexers.back().get();

        // TODO: Count source file reference and dealloc its memory

    }

    glsl_tok::GLSLPPKeywordKind getPPKeywordKind(const char *keyword, unsigned len) {

#define HASH(LEN, A, B) \
    (((LEN) << 5) + ((((A) - 'a') + ((B) - 'a')) & 31))
#define CASE_UNIFORM(LEN, FIRST, THIRD, NAME) \
    case HASH(LEN, FIRST, THIRD): \
        return memcmp(keyword, #NAME, LEN) ? glsl_tok::pp_not_keyword : glsl_tok::pp_ ## NAME;

        switch (HASH(len, keyword[0], keyword[2])) {

            CASE_UNIFORM(7, 'i', 'c', include)
            CASE_UNIFORM(7, 'v', 'r', version)

            default: return glsl_tok::pp_not_keyword;
        }

#undef CASE_UNIFORM
#undef HASH

    }

    const char *glsl_common =
#include "shaders/common.glsl"
    ;
    const char *glsl_lights_preamble =
#include "shaders/lights_preamble.glsl"
    ;

    const char *glsl_begin_vert =
#include "shaders/begin_vert.glsl"
    ;
    const char *glsl_worldposition_vert =
#include "shaders/worldposition_vert.glsl"
    ;
    const char *glsl_worldnormal_vert =
#include "shaders/worldnormal_vert.glsl"
    ;
    const char *glsl_glposition_vert =
#include "shaders/glposition_vert.glsl"
    ;

    void GLSLPreprocessor::handleDirective(GLSLToken &token) {

        GLSLToken hash = token;

        // Get the keyword
        lex(token);

        if (token.kind == glsl_tok::eod || token.len < 2) { // Empty line or not a directive

            NotDirective:

            // Skip to the end of line
            while (token.kind != glsl_tok::eod) lex(token);

            token.kind = glsl_tok::unknown;
            token.len = static_cast<unsigned>(token.pointer - hash.pointer);
            token.pointer = hash.pointer;

            return;

        }

        // Make cleaned keyword
        char keyword[token.len + 1];
        memcpy(keyword, token.pointer, token.len);
        keyword[token.len] = '\0';

        switch (getPPKeywordKind(keyword, token.len)) {
            default:
                break;

            case glsl_tok::pp_include: { // Include directive

                currentLexer->isLexingFilename = true;
                lex(token);
                currentLexer->isLexingFilename = false;

                GLSLToken fileToken = token;

                // Expect next token to be end of directive
                lex(token);
                if (token.kind != glsl_tok::eod) goto NotDirective;

                // Expecting a string literal or an angled string literal
                switch (fileToken.kind) {
                    default:
                        break;

                    case glsl_tok::string_literal:
                    case glsl_tok::angle_string_literal: {

                        unsigned fileLen = fileToken.len - 2;
                        char file[fileLen + 1];
                        memcpy(file, fileToken.pointer + 1, fileLen);
                        file[fileLen] = '\0';

                        if (fileToken.kind == glsl_tok::angle_string_literal) {
                            // Built in fragments

#define HASH(LEN, A, B) \
    (((LEN) << 5) + ((((A) - 'a') + ((B) - 'a')) & 31))
#define CASE_UNIFORM(LEN, FIRST, THIRD, NAME) \
    case HASH(LEN, FIRST, THIRD): \
        if (memcmp(file, #NAME, LEN)) break; enterSource(glsl_ ## NAME); lex(token); return;

                            switch (HASH(fileLen, file[0], file[2])) {

                                CASE_UNIFORM(6, 'c', 'm', common)

                                CASE_UNIFORM(15, 'l', 'g', lights_preamble)

                                // Vertex shader

                                CASE_UNIFORM(10, 'b', 'g', begin_vert)

                                CASE_UNIFORM(18, 'w', 'r', worldposition_vert)

                                CASE_UNIFORM(16, 'w', 'r', worldnormal_vert)

                                CASE_UNIFORM(15, 'g', 'p', glposition_vert)

                                default:
                                    break;
                            }

#undef CASE_UNIFORM
#undef HASH

                        } else {

                            // TODO: Search file from relative path

                        }

                    }

                }

                break;

            }

            case glsl_tok::pp_version:

                // TODO: Have something similar to isLexingFilename for number literal
                lex(token); // Version number

                // Expect next token to be end of directive
                lex(token);
                if (token.kind != glsl_tok::eod) goto NotDirective;

                // Next to lex from bootstrap script
                enterSource(bootstrap);

        }

        goto NotDirective;

    }

    std::string preprocessGLSLSource(const char *source, const std::string &bootstrap) {
        std::string result;
        GLSLPreprocessor preprocessor;

        preprocessor.bootstrap = bootstrap;
        preprocessor.enterMainSource(source);

        GLSLToken token{};
        do {
            preprocessor.lex(token);

            result.append(token.pointer, token.len);
            result.append("\n");

        } while (token.kind != glsl_tok::eof);

        result.append("\0");

        return result;
    }

    std::string readFile(const char *filename) {
        std::ifstream f(filename);
        std::string result((std::istreambuf_iterator<char>(f)),
                           (std::istreambuf_iterator<char>()));
        return result;
    }

    std::string preprocessGLSLSourceFile(const char *filename, const std::string &bootstrap) {
        std::string result;
        GLSLPreprocessor preprocessor;

        preprocessor.bootstrap = bootstrap;
        preprocessor.enterMainSourceFile(filename);

        GLSLToken token{};
        do {
            preprocessor.lex(token);

            result.append(token.pointer, token.len);
            result.append("\n");

        } while (token.kind != glsl_tok::eof);

        result.append("\0");

        return result;
    }

    GLSLPreprocessorLexer::GLSLPreprocessorLexer(GLSLPreprocessor *preprocessor,
                                                 const char *bufferStart,
                                                 const char *bufferPointer)
        : preprocessor(preprocessor), bufferStart(bufferStart), bufferPointer(bufferPointer),
          isAtPhysicalStartOfLine(true), isPreprocessingDirective(false), isLexingFilename(false),
          line(0) {

    }

    bool GLSLPreprocessorLexer::lex(GLSLToken &token) {

        LexNextToken:

        bool atPhysicalStartOfLine = false;
        if (isAtPhysicalStartOfLine) {
            atPhysicalStartOfLine = true;
            isAtPhysicalStartOfLine = false;
            ++line;
        }

        skipHorizontalWhitespace(token);

        // Get and advance pointer
        const char *pointer = bufferPointer;

        switch (*pointer++) {
            default: {
                // A normal line of code

                LexLine:

                return lexLine(token, pointer);

            }

            case '\0': // End of file

                if (isPreprocessingDirective) {
                    // Append eod before the end of file

                    token.kind = glsl_tok::eod;
                    token.pointer = bufferPointer;
                    token.len = 1;

                    isPreprocessingDirective = false;

                    break;
                }

                token.kind = glsl_tok::eof;
                token.pointer = bufferPointer;
                token.len = 1;

                bufferPointer = pointer;

                break;

            case '\n':

                isAtPhysicalStartOfLine = true;

                if (isPreprocessingDirective) {
                    token.kind = glsl_tok::eod;
                    token.pointer = bufferPointer;
                    token.len = 1;

                    bufferPointer = pointer;
                    isPreprocessingDirective = false;

                    break;
                }

                bufferPointer = pointer;

                goto LexNextToken;

            case '#': // Directive

                if (atPhysicalStartOfLine) {

                    // Hash
                    token.kind = glsl_tok::unknown;
                    token.pointer = bufferPointer;
                    token.len = 1;

                    bufferPointer = pointer;

                    isPreprocessingDirective = true;

                    preprocessor->handleDirective(token);

                    break;

                }

                goto LexLine;

            case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
            case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N':
            case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U':
            case 'V': case 'W': case 'X': case 'Y': case 'Z':
            case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
            case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
            case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u':
            case 'v': case 'w': case 'x': case 'y': case 'z':
            case '_':

                // Only lex identifier if in directive
                if (!isPreprocessingDirective) goto LexLine;
                return lexIdentifier(token, pointer);

            case '"': // String literal

                // Only lex literal if in directive
                if (!isPreprocessingDirective) goto LexLine;
                return lexStringLiteral(token, pointer);

            case '<':

                if (!isPreprocessingDirective) goto LexLine;
                if (isLexingFilename) { // Preprocesser lexing a filename
                    return lexAngledStringLiteral(token, pointer);
                }
                goto LexLine;

        }

        return true;

    }

    bool GLSLPreprocessorLexer::lexLine(GLSLToken &token, const char *pointer) {

        while (*pointer != '\0' && *pointer != '\n') ++pointer;

        if (pointer == bufferPointer) return false;

        token.kind = glsl_tok::unknown;
        token.pointer = bufferPointer;
        token.len = static_cast<unsigned>(pointer - bufferPointer);

        bufferPointer = pointer;

        return true;

    }

    bool GLSLPreprocessorLexer::lexIdentifier(GLSLToken &token, const char *pointer) {

        while (isIdentifierBody(*pointer)) ++pointer;

        if (pointer == bufferPointer) return false;

        token.kind = glsl_tok::raw_identifier;
        token.pointer = bufferPointer;
        token.len = static_cast<unsigned>(pointer - bufferPointer);

        bufferPointer = pointer;

        return true;

    }

    bool GLSLPreprocessorLexer::lexStringLiteral(GLSLToken &token, const char *pointer) {

        while (*pointer != '"') {

            // Escape character
            if (*pointer == '\\') ++pointer;

            if (*pointer == '\0' || *pointer == '\n') { // Early termination of string

                token.kind = glsl_tok::unknown;
                token.pointer = bufferPointer;
                token.len = static_cast<unsigned>(pointer - bufferPointer);

                bufferPointer = pointer;

                break;
            }

            ++pointer;

        }

        ++pointer;

        token.kind = glsl_tok::string_literal;
        token.pointer = bufferPointer;
        token.len = static_cast<unsigned>(pointer - bufferPointer);

        bufferPointer = pointer;

        return true;

    }

    bool GLSLPreprocessorLexer::lexAngledStringLiteral(GLSLToken &token, const char *pointer) {

        while (*pointer != '>') {

            // Escape character
            if (*pointer == '\\') ++pointer;

            if (*pointer == '\0' || *pointer == '\n') { // Early termination of string

                token.kind = glsl_tok::unknown;
                token.pointer = bufferPointer;
                token.len = static_cast<unsigned>(pointer - bufferPointer);

                bufferPointer = pointer;

                break;
            }

            ++pointer;

        }

        ++pointer;

        token.kind = glsl_tok::angle_string_literal;
        token.pointer = bufferPointer;
        token.len = static_cast<unsigned>(pointer - bufferPointer);

        bufferPointer = pointer;

        return true;

    }

    bool GLSLPreprocessorLexer::skipHorizontalWhitespace(GLSLToken &token) {

        const char *pointer = bufferPointer;

        while (isHorizontalWhitespace(*pointer)) ++pointer;

        if (pointer == bufferPointer) return false;

        token.kind = glsl_tok::unknown;
        token.pointer = bufferPointer;
        token.len = static_cast<unsigned>(pointer - bufferPointer);

        bufferPointer = pointer;

        return true;

    }

}


#include <iostream>
#include "GLSLPreprocessor.h"


namespace renderbox {

    void GLSLPreprocessor::lex(GLSLToken &token) {

        bool returnedToken;
        do {
            returnedToken = currentLexer->lex(token);

            if (token.kind == eof) {
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

        GLSLPreprocessorSource *source;
        auto it = sources.find(filename);
        if (it == sources.end()) {
            source = new GLSLPreprocessorSource(filename);
            sources.insert(std::pair<const char *, std::unique_ptr<GLSLPreprocessorSource>>(
                filename, std::unique_ptr<GLSLPreprocessorSource>(source)));
        } else {
            source = it->second.get();
        }

        const char *bufferStart = source->source.get();

        currentLexer = new GLSLPreprocessorLexer(this, bufferStart, bufferStart);
        lexers.push_back(std::unique_ptr<GLSLPreprocessorLexer>(currentLexer));

    }

    void GLSLPreprocessor::enterSource(const char *source) {

        const char *bufferStart = source;

        currentLexer = new GLSLPreprocessorLexer(this, bufferStart, bufferStart);
        lexers.push_back(std::unique_ptr<GLSLPreprocessorLexer>(currentLexer));

    }

    void GLSLPreprocessor::exitSource() {

        lexers.pop_back();
        currentLexer = lexers.back().get();

        // TODO: Count source file reference and dealloc its memory

    }

    GLSLPPKeywordKind getPPKeywordKind(const char *keyword, unsigned len) {

#define HASH(LEN, FIRST, THIRD) \
    (((LEN) << 5) + ((((FIRST) - 'a') + ((THIRD) - 'a')) & 31))
#define CASE(LEN, FIRST, THIRD, NAME) \
    case HASH(LEN, FIRST, THIRD): \
        return memcmp(keyword, #NAME, LEN) ? pp_not_keyword : pp_ ## NAME

        switch (HASH(len, keyword[0], keyword[2])) {

            CASE(7, 'i', 'c', include);

            default: return pp_not_keyword;
        }

#undef CASE
#undef HASH

    }

    const char *glsl_common =
#include "shaders/common.glsl"
    ;

    void GLSLPreprocessor::handleDirective(GLSLToken &token) {

        GLSLToken hash = token;

        // Get the keyword
        lex(token);

        if (token.kind == eod || token.len < 2) { // Empty line or not a directive

            NotDirective:

            // Skip to the end of line
            while (token.kind != eod) lex(token);

            token.kind = unknown;
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

            case pp_include: // Include directive

                currentLexer->isLexingFilename = true;
                lex(token);
                currentLexer->isLexingFilename = false;

                GLSLToken fileToken = token;

                // Expect next token to be end of directive
                lex(token);
                if (token.kind != eod) goto NotDirective;

                // Expecting a string literal or an angled string literal
                switch (fileToken.kind) {
                    default: break;

                    case string_literal:
                    case angle_string_literal: {

                        unsigned fileLen = fileToken.len - 2;
                        char file[fileLen + 1];
                        memcpy(file, fileToken.pointer + 1, fileLen);
                        file[fileLen] = '\0';

                        if (fileToken.kind == angle_string_literal) {
                            // Built in fragments

#define HASH(LEN, FIRST, THIRD) \
    (((LEN) << 5) + ((((FIRST) - 'a') + ((THIRD) - 'a')) & 31))
#define CASE(LEN, FIRST, THIRD, NAME) \
    case HASH(LEN, FIRST, THIRD): \
        if (memcmp(file, #NAME, LEN)) break; enterSource(glsl_ ## NAME); lex(token); return

                            switch (HASH(fileLen, file[0], file[2])) {

                                CASE(6, 'c', 'm', common);

                                default: break;
                            }

#undef CASE
#undef HASH

                        } else {

                            // TODO: Search file from relative path

                        }

                    }

                }

        }

        goto NotDirective;

    }

    GLSLPreprocessorSource::GLSLPreprocessorSource(const char *filename) {

        FILE *f = fopen(filename, "r");

        // Determine file size
        fseek(f, 0, SEEK_END);
        long size_ = ftell(f);

        if (size_ < 0) throw 2; // Failed to tell size
        size = static_cast<size_t>(size_);

        source.reset(new char[size]);

        rewind(f);
        fread(source.get(), sizeof(char), size, f);

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

                    token.kind = eod;
                    token.pointer = bufferPointer;
                    token.len = 1;

                    isPreprocessingDirective = false;

                    break;
                }

                token.kind = eof;
                token.pointer = bufferPointer;
                token.len = 1;

                bufferPointer = pointer;

                break;

            case '\n':

                isAtPhysicalStartOfLine = true;

                if (isPreprocessingDirective) {
                    token.kind = eod;
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
                    token.kind = unknown;
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

        token.kind = unknown;
        token.pointer = bufferPointer;
        token.len = static_cast<unsigned>(pointer - bufferPointer);

        bufferPointer = pointer;

        return true;

    }

    bool GLSLPreprocessorLexer::lexIdentifier(GLSLToken &token, const char *pointer) {

        while (isIdentifierBody(*pointer)) ++pointer;

        if (pointer == bufferPointer) return false;

        token.kind = identifier;
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

                token.kind = unknown;
                token.pointer = bufferPointer;
                token.len = static_cast<unsigned>(pointer - bufferPointer);

                bufferPointer = pointer;

                break;
            }

            ++pointer;

        }

        ++pointer;

        token.kind = string_literal;
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

                token.kind = unknown;
                token.pointer = bufferPointer;
                token.len = static_cast<unsigned>(pointer - bufferPointer);

                bufferPointer = pointer;

                break;
            }

            ++pointer;

        }

        ++pointer;

        token.kind = angle_string_literal;
        token.pointer = bufferPointer;
        token.len = static_cast<unsigned>(pointer - bufferPointer);

        bufferPointer = pointer;

        return true;

    }

    bool GLSLPreprocessorLexer::skipHorizontalWhitespace(GLSLToken &token) {

        const char *pointer = bufferPointer;

        while (isHorizontalWhitespace(*pointer)) ++pointer;

        if (pointer == bufferPointer) return false;

        token.kind = unknown;
        token.pointer = bufferPointer;
        token.len = static_cast<unsigned>(pointer - bufferPointer);

        bufferPointer = pointer;

        return true;

    }

    std::ostream &operator<<(std::ostream &os, const GLSLToken &token) {

        char cstr[token.len + 1];
        memcpy(cstr, token.pointer, token.len);
        cstr[token.len] = '\0';

        os << cstr;

        return os;

    }
}

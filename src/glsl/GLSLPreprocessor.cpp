
#include "GLSLPreprocessor.h"


namespace renderbox {

    void GLSLPreprocessor::lex(GLSLToken &token) {

        bool returnedToken;
        do {
            returnedToken = currentLexer->lex(token);
        } while (!returnedToken);

    }

    void GLSLPreprocessor::enterMainSource(const char *filename) {
        enterSource(filename);
    }

    void GLSLPreprocessor::enterSource(const char *filename) {

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
        const char *bufferEnd = bufferStart + source->size;

        currentLexer = new GLSLPreprocessorLexer(this, bufferStart, bufferStart, bufferEnd);
        lexers.push_back(std::unique_ptr<GLSLPreprocessorLexer>(currentLexer));

    }

    void GLSLPreprocessor::exitSource() {

        lexers.pop_back();
        currentLexer = lexers.back().get();

        // TODO: Count source file reference and dealloc its memory

    }

    bool GLSLPreprocessor::handleDirective(GLSLToken &token) {

        return false;

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
                                                 const char *bufferPointer,
                                                 const char *bufferEnd)
        : preprocessor(preprocessor), bufferStart(bufferStart), bufferPointer(bufferPointer), bufferEnd(bufferEnd) {
        isAtPhysicalStartOfLine = true;
    }

    bool GLSLPreprocessorLexer::lex(GLSLToken &token) {

        LexNextToken:

        skipWhitespace(token);

        // Get character and advance pointer
        const char *pointer = bufferPointer++;

        switch (*pointer) {
            case 0: // End of file
                token.kind = eof;
                token.pointer = pointer;
                token.len = 1;
                break;

            case '\n':
                isAtPhysicalStartOfLine = true;
                goto LexNextToken;

            case '#': // Directive
                token.kind = unknown;
                token.pointer = pointer;
                token.len = 1;

                preprocessor->handleDirective(token);
                // Preprocessor may fail to handle directive, then return the hash only

                break;

            default:
                // A normal line of code

                --bufferPointer; // Back off

                while (*pointer != '\n') ++pointer;

                token.kind = unknown;
                token.pointer = bufferPointer;
                token.len = static_cast<unsigned>(pointer - bufferPointer);

                bufferPointer = pointer;

                break;
        }

        return true;

    }

    bool GLSLPreprocessorLexer::skipWhitespace(GLSLToken &token) {

        const char *pointer = bufferPointer;

        while (isHorizontalWhitespace(*pointer)) ++pointer;

        if (pointer != bufferPointer) {

            token.kind = unknown;
            token.pointer = bufferPointer;
            token.len = static_cast<unsigned>(pointer - bufferPointer);

            bufferPointer = pointer;

            return true;

        }

        return false;

    }

}
#include <iostream>
#include "OBJLoader.h"


namespace renderbox {

    std::ostream &operator<<(std::ostream &os, const OBJToken &token) {

        char cstr[token.len + 1];
        memcpy(cstr, token.pointer, token.len);
        cstr[token.len] = '\0';

        os << cstr;

        return os;

    }

    OBJLexer::OBJLexer(const char *bufferStart, const char *bufferPointer)
        : bufferStart(bufferStart), bufferPointer(bufferPointer),
          isAtPhysicalStartOfLine(true), isLexingFilename(true), line(0) {

    }

    bool OBJLexer::lex(OBJToken &token) {

        LexNextToken:

        bool atPhysicalStartOfLine = false;
        if (isAtPhysicalStartOfLine) {
            atPhysicalStartOfLine = true;
            isAtPhysicalStartOfLine = false;
            ++line;
        }

        skipHorizontalWhitespace(token);

        const char *pointer = bufferPointer;

        switch (*pointer++) {
            default: {

                // Unknown

                std::cout << "$" << *(pointer - 1) << std::endl;

                bufferPointer = pointer;

                break;

            }

            case '\0': // End of file

                token.kind = obj_tok::eof;
                token.pointer = bufferPointer;
                token.len = 1;

                bufferPointer = pointer;

                break;

            case '\n':

                token.kind = obj_tok::eol;
                token.pointer = bufferPointer;
                token.len = 1;

                isAtPhysicalStartOfLine = true;

                bufferPointer = pointer;

                break;

            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':

                return lexNumericConstant(token, pointer);

            case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
            case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N':
            case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U':
            case 'V': case 'W': case 'X': case 'Y': case 'Z':
            case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
            case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
            case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u':
            case 'v': case 'w': case 'x': case 'y': case 'z':
            case '_':

                if (isLexingFilename) { // Lexing a filename without quotes etc
                    return lexUnquotedStringLiteral(token, pointer);
                }
                return lexIdentifier(token, pointer);

            case '-':

                token.kind = obj_tok::minus;
                token.pointer = bufferPointer;
                token.len = static_cast<unsigned>(pointer - bufferPointer);

                bufferPointer = pointer;

                break;

            case '/':

                token.kind = obj_tok::slash;
                token.pointer = bufferPointer;
                token.len = static_cast<unsigned>(pointer - bufferPointer);

                bufferPointer = pointer;

                break;

            case '#':

                return skipLineComment(token, pointer);

        }

        return true;

    }

    bool OBJLexer::lexNumericConstant(OBJToken &token, const char *pointer) {

        bool period = false;

        LexDigits:

        while ((CHAR_ASCII_INFO[*pointer] & CHAR_DIGIT) != 0) ++pointer;

        if (!period && (CHAR_ASCII_INFO[*pointer] & CHAR_PERIOD)) {
            period = true;
            ++pointer;
            goto LexDigits;
        }

        if (pointer == bufferPointer) return false;

        token.kind = obj_tok::numeric_constant;
        token.pointer = bufferPointer;
        token.len = static_cast<unsigned>(pointer - bufferPointer);

        bufferPointer = pointer;

        return true;

    }

    bool OBJLexer::lexIdentifier(OBJToken &token, const char *pointer) {

        while (isIdentifierBody(*pointer)) ++pointer;

        if (pointer == bufferPointer) return false;

        token.kind = obj_tok::raw_identifier;
        token.pointer = bufferPointer;
        token.len = static_cast<unsigned>(pointer - bufferPointer);

        bufferPointer = pointer;

        return true;

    }

    bool OBJLexer::lexUnquotedStringLiteral(OBJToken &token, const char *pointer) {

        while (!isHorizontalWhitespace(*pointer) &&
               *pointer != '\0' && *pointer != '\n') ++pointer;

        if (pointer == bufferPointer) return false;

        token.kind = obj_tok::unquoted_string_literal;
        token.pointer = bufferPointer;
        token.len = static_cast<unsigned>(pointer - bufferPointer);

        bufferPointer = pointer;

        return true;

    }

    bool OBJLexer::skipLineComment(OBJToken &token, const char *pointer) {

        while (*pointer != '\0' && *pointer != '\n') ++pointer;

        if (pointer == bufferPointer) return false;

        token.kind = obj_tok::comment;
        token.pointer = bufferPointer;
        token.len = static_cast<unsigned>(pointer - bufferPointer);

        bufferPointer = pointer;

        return true;

    }

    bool OBJLexer::skipHorizontalWhitespace(OBJToken &token) {

        const char *pointer = bufferPointer;

        while (isHorizontalWhitespace(*pointer)) ++pointer;

        if (pointer == bufferPointer) return false;

        token.kind = obj_tok::unknown;
        token.pointer = bufferPointer;
        token.len = static_cast<unsigned>(pointer - bufferPointer);

        bufferPointer = pointer;

        return true;

    }

}

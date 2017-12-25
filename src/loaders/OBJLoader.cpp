#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include "MeshLambertMaterial.h"
#include "OBJLoader.h"


#define INVALID_SYNTAX() \
    throw 2;
#define INVALID_STATE() \
    throw 2;

namespace renderbox {

    std::ostream &operator<<(std::ostream &os, const OBJToken &token) {

        char cstr[token.len + 1];
        memcpy(cstr, token.pointer, token.len);
        cstr[token.len] = '\0';

        os << cstr;

        return os;

    }

    OBJLoader::OBJLoader(std::shared_ptr<Object> destination)
        : destination(std::move(destination)), object(nullptr), geometry(nullptr), material(nullptr) {

    }

    void OBJLoader::enterOBJSourceFile(const char *filename) {

        if (!sourceFiles.empty()) {
            // Resolve relative path

            // Preserve last slash
            std::string relative(sourceFiles.back());
            auto separator = relative.find_last_of('/'); // Return -1 if not found
            relative.resize(separator + 1);
            // Append file name
            relative.append(filename);

            filename = relative.c_str();

        }

        sourceFiles.emplace_back(filename);

        Source source(filename);
        enterOBJSource(source.source.get());

        sourceFiles.pop_back();

    }

    void OBJLoader::enterMTLSourceFile(const char *filename) {

        if (!sourceFiles.empty()) {
            // Resolve relative path

            // Preserve last slash
            std::string relative(sourceFiles.back());
            auto separator = relative.find_last_of('/'); // Return -1 if not found
            relative.resize(separator + 1);
            // Append file name
            relative.append(filename);

            filename = relative.c_str();

        }

        sourceFiles.emplace_back(filename);

        Source source(filename);
        enterMTLSource(source.source.get());

        sourceFiles.pop_back();

    }

    void OBJLoader::enterOBJSource(const char *source) {

        OBJLexer lexer(source, source);
        OBJToken token{};

        NextLine:

        lex(lexer, token);

        switch (token.kind) {
            default:                 break;
            case obj_tok::eof:       return;
            case obj_tok::kw_v:      handleVertex(lexer, token); break; // Geometric vertex
            case obj_tok::kw_vt:     handleTextureVertex(lexer, token); break; // Texture vertex
            case obj_tok::kw_vn:     handleVertexNormal(lexer, token); break; // Vertex normal
            case obj_tok::kw_f:      handleFace(lexer, token); break; // Face
            case obj_tok::kw_o:      handleObject(lexer, token); break; // Object
            case obj_tok::kw_mtllib: handleMaterialLibrary(lexer, token); break; // Material library
            case obj_tok::kw_usemtl: handleMaterialName(lexer, token); break; // Material name
        }

        goto NextLine;

    }

    void OBJLoader::enterMTLSource(const char *source) {

        MTLLexer lexer(source, source);
        MTLToken token{};

        NextLine:

        lex(lexer, token);

        switch (token.kind) {
            default:                 break;
            case mtl_tok::eof:       return;
            case mtl_tok::kw_newmtl: handleNewMaterial(lexer, token); break; // Material name
            case mtl_tok::kw_Ka:
            case mtl_tok::kw_Kd:
            case mtl_tok::kw_Ks:     handleMaterialColor(lexer, token); break; // Material color
        }

        goto NextLine;

    }

    void OBJLoader::lex(OBJLexer &lexer, OBJToken &token) {

        bool returnedToken;
        do {
            returnedToken = lexer.lex(token);
        } while (!returnedToken);

    }

    void OBJLoader::lex(MTLLexer &lexer, MTLToken &token) {

        bool returnedToken;
        do {
            returnedToken = lexer.lex(token);
        } while (!returnedToken);

    }

    void OBJLoader::handleVertex(OBJLexer &lexer, OBJToken &token) {

        glm::vec3 v;

        // Expect x coordinate
        lex(lexer, token);
        if (token.kind != obj_tok::numeric_constant && token.kind != obj_tok::minus) INVALID_SYNTAX();
        v.x = parseFloat(lexer, token);

        // Expect y coordinate
        lex(lexer, token);
        if (token.kind != obj_tok::numeric_constant && token.kind != obj_tok::minus) INVALID_SYNTAX();
        v.y = parseFloat(lexer, token);

        // Expect z coordinate
        lex(lexer, token);
        if (token.kind != obj_tok::numeric_constant && token.kind != obj_tok::minus) INVALID_SYNTAX();
        v.z = parseFloat(lexer, token);

        // Expect w coordinate (optional)
        lex(lexer, token);
        if (token.kind == obj_tok::numeric_constant || token.kind == obj_tok::minus) {
            v /= parseFloat(lexer, token);

            lex(lexer, token);
        }

        // Expect end of line
        if (token.kind != obj_tok::eol) INVALID_SYNTAX();

        vertices.emplace_back(v);

    }

    void OBJLoader::handleTextureVertex(OBJLexer &lexer, OBJToken &token) {

        glm::vec2 v;

        // Expect u
        lex(lexer, token);
        if (token.kind != obj_tok::numeric_constant && token.kind != obj_tok::minus) INVALID_SYNTAX();
        v.x = parseFloat(lexer, token);

        // Expect v
        lex(lexer, token);
        if (token.kind != obj_tok::numeric_constant && token.kind != obj_tok::minus) INVALID_SYNTAX();
        v.y = parseFloat(lexer, token);

        // Expect end of line
        lex(lexer, token);
        if (token.kind != obj_tok::eol) INVALID_SYNTAX();

        uvs.emplace_back(v);

    }

    void OBJLoader::handleVertexNormal(OBJLexer &lexer, OBJToken &token) {

        glm::vec3 v;

        // Expect x coordinate
        lex(lexer, token);
        if (token.kind != obj_tok::numeric_constant && token.kind != obj_tok::minus) INVALID_SYNTAX();
        v.x = parseFloat(lexer, token);

        // Expect y coordinate
        lex(lexer, token);
        if (token.kind != obj_tok::numeric_constant && token.kind != obj_tok::minus) INVALID_SYNTAX();
        v.y = parseFloat(lexer, token);

        // Expect z coordinate
        lex(lexer, token);
        if (token.kind != obj_tok::numeric_constant && token.kind != obj_tok::minus) INVALID_SYNTAX();
        v.z = parseFloat(lexer, token);

        // Expect end of line
        lex(lexer, token);
        if (token.kind != obj_tok::eol) INVALID_SYNTAX();

        normals.emplace_back(v);

    }

    void OBJLoader::handleFace(OBJLexer &lexer, OBJToken &token) {

        if (object == nullptr) INVALID_STATE();

        std::vector<unsigned> v, vt, vn;
        bool bvt = false, bvn = false;

        // Expect v1
        lex(lexer, token);
        if (token.kind != obj_tok::numeric_constant) INVALID_SYNTAX();
        v.emplace_back(parseInt(lexer, token));

        // Expect slash (optional)
        lex(lexer, token);
        if (token.kind == obj_tok::slash) {

            // Expect vt1 or slash
            lex(lexer, token);
            if (token.kind == obj_tok::numeric_constant) {
                bvt = true;
                vt.emplace_back(parseInt(lexer, token));

                // Expect slash or next part
                lex(lexer, token);
                if (token.kind == obj_tok::slash) goto ExpectVN1;
            } else if (token.kind == obj_tok::slash) {

                ExpectVN1:

                lex(lexer, token);
                if (token.kind != obj_tok::numeric_constant) INVALID_SYNTAX();

                bvn = true;
                vn.emplace_back(parseInt(lexer, token));

                lex(lexer, token);
            } else INVALID_SYNTAX();
        } else lex(lexer, token);

        // Expect v2
        if (token.kind != obj_tok::numeric_constant) INVALID_SYNTAX();
        v.emplace_back(parseInt(lexer, token));

        if (bvt || bvn) {
            // Expect slash (only if bvt or bvn)
            lex(lexer, token);
            if (token.kind != obj_tok::slash) INVALID_SYNTAX();

            if (bvt) {
                // Expect vt2 (if bvt)
                lex(lexer, token);
                if (token.kind != obj_tok::numeric_constant) INVALID_SYNTAX();
                vt.emplace_back(parseInt(lexer, token));
            }

            if (bvn) {
                // Expect slash (only if bvn)
                lex(lexer, token);
                if (token.kind != obj_tok::slash) INVALID_SYNTAX();

                // Expect vn2
                lex(lexer, token);
                if (token.kind != obj_tok::numeric_constant) INVALID_SYNTAX();
                vn.emplace_back(parseInt(lexer, token));
            }
        }

        lex(lexer, token);

        do {

            // Expect v3
            if (token.kind != obj_tok::numeric_constant) INVALID_SYNTAX();
            v.emplace_back(parseInt(lexer, token));

            if (bvt || bvn) {
                // Expect slash (only if bvt or bvn)
                lex(lexer, token);
                if (token.kind != obj_tok::slash) INVALID_SYNTAX();

                if (bvt) {
                    // Expect vt3 (if bvt)
                    lex(lexer, token);
                    if (token.kind != obj_tok::numeric_constant) INVALID_SYNTAX();
                    vt.emplace_back(parseInt(lexer, token));
                }

                if (bvn) {
                    // Expect slash (only if bvn)
                    lex(lexer, token);
                    if (token.kind != obj_tok::slash) INVALID_SYNTAX();

                    // Expect vn3
                    lex(lexer, token);
                    if (token.kind != obj_tok::numeric_constant) INVALID_SYNTAX();
                    vn.emplace_back(parseInt(lexer, token));
                }
            }

            lex(lexer, token);

        } while (token.kind != obj_tok::eol); // Expect end of line

        if (v.size() > 4) INVALID_SYNTAX(); // Only support triangle and quad

        geometry->getFaces().emplace_back(glm::uvec3(
            getVertexIndex(v[0] - 1, (bvt ? vt[0] : 0) - 1, (bvn ? vn[0] : 0) - 1),
            getVertexIndex(v[1] - 1, (bvt ? vt[1] : 0) - 1, (bvn ? vn[1] : 0) - 1),
            getVertexIndex(v[2] - 1, (bvt ? vt[2] : 0) - 1, (bvn ? vn[2] : 0) - 1)));

        if (v.size() == 4) { // Quad
            geometry->getFaces().emplace_back(glm::uvec3(
                getVertexIndex(v[2] - 1, (bvt ? vt[2] : 0) - 1, (bvn ? vn[2] : 0) - 1),
                getVertexIndex(v[3] - 1, (bvt ? vt[3] : 0) - 1, (bvn ? vn[3] : 0) - 1),
                getVertexIndex(v[0] - 1, (bvt ? vt[0] : 0) - 1, (bvn ? vn[0] : 0) - 1)));
        }

    }

    void OBJLoader::handleObject(OBJLexer &lexer, OBJToken &token) {

        geometry = new Geometry;
        object = new Object(std::shared_ptr<Geometry>(geometry), nullptr);
        destination->addChild(std::shared_ptr<Object>(object));

        geometryVertexIndices.clear();

    }

    void OBJLoader::handleMaterialLibrary(OBJLexer &lexer, OBJToken &token) {

        lexer.isLexingFilename = true;

        lex(lexer, token);
        do {
            if (token.kind != obj_tok::unquoted_string_literal) INVALID_SYNTAX();

            // Make cleaned library
            char library[token.len + 1];
            memcpy(library, token.pointer, token.len);
            library[token.len] = '\0';

            // Load material library
            enterMTLSourceFile(library);

            lex(lexer, token);
        } while (token.kind != obj_tok::eol);

        lexer.isLexingFilename = false;

    }

    void OBJLoader::handleMaterialName(OBJLexer &lexer, OBJToken &token) {

        // Expect material name
        lexer.isLexingMaterialName = true;
        lex(lexer, token);
        lexer.isLexingMaterialName = false;
        if (token.kind != obj_tok::unquoted_string_literal) INVALID_SYNTAX();

        // Make cleaned name
        char name[token.len + 1];
        memcpy(name, token.pointer, token.len);
        name[token.len] = '\0';

        // Expect eol
        lex(lexer, token);
        if (token.kind != obj_tok::eol) INVALID_SYNTAX();

        // Material name should exist
        auto it = materials.find(name);
        if (it == materials.end()) INVALID_STATE();

        object->setMaterial(it->second);

    }

    void OBJLoader::handleNewMaterial(MTLLexer &lexer, MTLToken &token) {

        // Expect material name
        lexer.isLexingMaterialName = true;
        lex(lexer, token);
        lexer.isLexingMaterialName = false;
        if (token.kind != mtl_tok::unquoted_string_literal) INVALID_SYNTAX();

        // Make cleaned name
        char name[token.len + 1];
        memcpy(name, token.pointer, token.len);
        name[token.len] = '\0';

        // Expect eol
        lex(lexer, token);
        if (token.kind != mtl_tok::eol) INVALID_SYNTAX();

        // Material name should not exist
        if (materials.find(name) != materials.end()) INVALID_STATE();

        material = new MeshLambertMaterial(glm::vec3(), glm::vec3()); // TODO: Propose generic/standard material
        materials.insert(std::make_pair(std::string(name),
                                        std::shared_ptr<Material>(material)));

    }

    void OBJLoader::handleMaterialColor(MTLLexer &lexer, MTLToken &token) {

        if (material == nullptr) INVALID_STATE();

        auto keyword = token;
        glm::vec3 v;

        // Expect r
        lex(lexer, token);
        if (token.kind != mtl_tok::numeric_constant && token.kind != mtl_tok::minus) INVALID_SYNTAX();
        v.x = parseFloat(lexer, token);

        // Expect g
        lex(lexer, token);
        if (token.kind != mtl_tok::numeric_constant && token.kind != mtl_tok::minus) INVALID_SYNTAX();
        v.y = parseFloat(lexer, token);

        // Expect b
        lex(lexer, token);
        if (token.kind != mtl_tok::numeric_constant && token.kind != mtl_tok::minus) INVALID_SYNTAX();
        v.z = parseFloat(lexer, token);

        // Expect eol
        lex(lexer, token);
        if (token.kind != mtl_tok::eol) INVALID_SYNTAX();

        switch (keyword.kind) {
            default: break;
            case mtl_tok::kw_Ka:
                if (auto m = dynamic_cast<AmbientMaterial *>(material)) m->setAmbientColor(v);
                break;
            case mtl_tok::kw_Kd:
                if (auto m = dynamic_cast<DiffuseMaterial *>(material)) m->setDiffuseColor(v);
                break;
        }

    }

    float OBJLoader::parseFloat(OBJLexer &lexer, OBJToken &token) {

        bool negative = false;

        if (token.kind == obj_tok::minus) {
            negative = true;

            lex(lexer, token);
            if (token.kind != obj_tok::numeric_constant) INVALID_SYNTAX();
        }

        // Make cleaned keyword
        char number[token.len + 1];
        memcpy(number, token.pointer, token.len);
        number[token.len] = '\0';

        return (negative ? -1 : 1) * strtof(number, nullptr);

    }

    float OBJLoader::parseFloat(MTLLexer &lexer, MTLToken &token) {

        bool negative = false;

        if (token.kind == mtl_tok::minus) {
            negative = true;

            lex(lexer, token);
            if (token.kind != mtl_tok::numeric_constant) INVALID_SYNTAX();
        }

        // Make cleaned keyword
        char number[token.len + 1];
        memcpy(number, token.pointer, token.len);
        number[token.len] = '\0';

        return (negative ? -1 : 1) * strtof(number, nullptr);

    }

    int OBJLoader::parseInt(OBJLexer &lexer, OBJToken &token) {

        bool negative = false;

        if (token.kind == obj_tok::minus) {
            negative = true;

            lex(lexer, token);
            if (token.kind != obj_tok::numeric_constant) INVALID_SYNTAX();
        }

        // Make cleaned keyword
        char number[token.len + 1];
        memcpy(number, token.pointer, token.len);
        number[token.len] = '\0';

        return static_cast<int>((negative ? -1 : 1) * strtol(number, nullptr, 10));

    }

    unsigned OBJLoader::getVertexIndex(int v, int vt, int vn) {

        auto key = std::make_tuple(v, vt, vn);

        auto it = geometryVertexIndices.find(key);
        if (it != geometryVertexIndices.end()) {
            return it->second;
        }

        geometry->getVertices().emplace_back(vertices[v]);
        if (vt >= 0) geometry->getUVs().emplace_back(uvs[vt]);
        if (vn >= 0) geometry->getNormals().emplace_back(normals[vn]);

        auto index = static_cast<unsigned>(geometry->getVertices().size() - 1);
        geometryVertexIndices[key] = index;

        return index;

    }

    OBJLexer::OBJLexer(const char *bufferStart, const char *bufferPointer)
        : bufferStart(bufferStart), bufferPointer(bufferPointer),
          isAtPhysicalStartOfLine(true), isLexingFilename(false), isLexingMaterialName(false), line(0) {

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

                bufferPointer = pointer;

                break;

            }

            case '\0': // End of file

                if (!atPhysicalStartOfLine) {
                    // Append eol before the end of file

                    token.kind = obj_tok::eol;
                    token.pointer = bufferPointer; // Actually points to eof
                    token.len = 1;

                    isAtPhysicalStartOfLine = true;

                    break;
                }

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

                if (isLexingFilename || isLexingMaterialName) { // Lexing a string without quotes etc
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

        // Make cleaned keyword
        char keyword[token.len + 1];
        memcpy(keyword, token.pointer, token.len);
        keyword[token.len] = '\0';

#define HASH(LEN, A, B) \
    (((LEN) << 5) + ((((A) - 'a') + ((B) - 'a')) & 31))
#define CASE_KEYWORD(LEN, FIRST, SECOND, NAME) \
    case HASH(LEN, FIRST, SECOND): if (memcmp(keyword, #NAME, LEN) == 0) token.kind = obj_tok::kw_##NAME; break;

        switch (HASH(token.len, keyword[0], keyword[1])) {

            CASE_KEYWORD(1, 'v', '\0', v)
            CASE_KEYWORD(1, 'f', '\0', f)
            CASE_KEYWORD(1, 'o', '\0', o)

            CASE_KEYWORD(2, 'v', 't', vt)
            CASE_KEYWORD(2, 'v', 'n', vn)

            CASE_KEYWORD(6, 'm', 't', mtllib)
            CASE_KEYWORD(6, 'u', 's', usemtl)

            default: break;
        }

#undef CASE_KEYWORD
#undef HASH

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

    MTLLexer::MTLLexer(const char *bufferStart, const char *bufferPointer)
        : bufferStart(bufferStart), bufferPointer(bufferPointer),
          isAtPhysicalStartOfLine(true), isLexingMaterialName(false), line(0){

    }

    bool MTLLexer::lex(MTLToken &token) {

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

                bufferPointer = pointer;

                break;

            }

            case '\0': // End of file

                if (!atPhysicalStartOfLine) {
                    // Append eol before the end of file

                    token.kind = mtl_tok::eol;
                    token.pointer = bufferPointer; // Actually points to eof
                    token.len = 1;

                    isAtPhysicalStartOfLine = true;

                    break;
                }

                token.kind = mtl_tok::eof;
                token.pointer = bufferPointer;
                token.len = 1;

                bufferPointer = pointer;

                break;

            case '\n':

                token.kind = mtl_tok::eol;
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

                if (isLexingMaterialName) { // Lexing a string without quotes etc
                    return lexUnquotedStringLiteral(token, pointer);
                }
                return lexIdentifier(token, pointer);

            case '-':

                token.kind = mtl_tok::minus;
                token.pointer = bufferPointer;
                token.len = static_cast<unsigned>(pointer - bufferPointer);

                bufferPointer = pointer;

                break;

            case '#':

                return skipLineComment(token, pointer);

        }

        return true;

    }

    bool MTLLexer::lexNumericConstant(MTLToken &token, const char *pointer) {

        bool period = false;

        LexDigits:

        while ((CHAR_ASCII_INFO[*pointer] & CHAR_DIGIT) != 0) ++pointer;

        if (!period && (CHAR_ASCII_INFO[*pointer] & CHAR_PERIOD)) {
            period = true;
            ++pointer;
            goto LexDigits;
        }

        if (pointer == bufferPointer) return false;

        token.kind = mtl_tok::numeric_constant;
        token.pointer = bufferPointer;
        token.len = static_cast<unsigned>(pointer - bufferPointer);

        bufferPointer = pointer;

        return true;

    }

    bool MTLLexer::lexIdentifier(MTLToken &token, const char *pointer) {

        while (isIdentifierBody(*pointer)) ++pointer;

        if (pointer == bufferPointer) return false;

        token.kind = mtl_tok::raw_identifier;
        token.pointer = bufferPointer;
        token.len = static_cast<unsigned>(pointer - bufferPointer);

        // Make cleaned keyword
        char keyword[token.len + 1];
        memcpy(keyword, token.pointer, token.len);
        keyword[token.len] = '\0';

#define HASH(LEN, A, B) \
    (((LEN) << 5) + ((((A) - 'a') + ((B) - 'a')) & 31))
#define CASE_KEYWORD(LEN, FIRST, SECOND, NAME) \
    case HASH(LEN, FIRST, SECOND): if (memcmp(keyword, #NAME, LEN) == 0) token.kind = mtl_tok::kw_##NAME; break;

        switch (HASH(token.len, keyword[0], keyword[1])) {

            CASE_KEYWORD(6, 'n', 'e', newmtl)

            CASE_KEYWORD(2, 'K', 'a', Ka)
            CASE_KEYWORD(2, 'K', 'd', Kd)
            CASE_KEYWORD(2, 'K', 's', Ks)

            default: break;
        }

#undef CASE_KEYWORD
#undef HASH

        bufferPointer = pointer;

        return true;

    }

    bool MTLLexer::lexUnquotedStringLiteral(MTLToken &token, const char *pointer) {

        while (!isHorizontalWhitespace(*pointer) &&
               *pointer != '\0' && *pointer != '\n') ++pointer;

        if (pointer == bufferPointer) return false;

        token.kind = mtl_tok::unquoted_string_literal;
        token.pointer = bufferPointer;
        token.len = static_cast<unsigned>(pointer - bufferPointer);

        bufferPointer = pointer;

        return true;

    }

    bool MTLLexer::skipLineComment(MTLToken &token, const char *pointer) {

        while (*pointer != '\0' && *pointer != '\n') ++pointer;

        if (pointer == bufferPointer) return false;

        token.kind = mtl_tok::comment;
        token.pointer = bufferPointer;
        token.len = static_cast<unsigned>(pointer - bufferPointer);

        bufferPointer = pointer;

        return true;

    }

    bool MTLLexer::skipHorizontalWhitespace(MTLToken &token) {

        const char *pointer = bufferPointer;

        while (isHorizontalWhitespace(*pointer)) ++pointer;

        if (pointer == bufferPointer) return false;

        token.kind = mtl_tok::unknown;
        token.pointer = bufferPointer;
        token.len = static_cast<unsigned>(pointer - bufferPointer);

        bufferPointer = pointer;

        return true;

    }

}

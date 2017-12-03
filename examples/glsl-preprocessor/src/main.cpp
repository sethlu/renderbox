
#include <iostream>
#include "renderbox.h"

int main(int argc, char** argv) {

    renderbox::GLSLPreprocessor preprocessor;
    renderbox::GLSLToken token;

    preprocessor.enterMainSource("shaders/shader.glsl");
    do {
        preprocessor.lex(token);

        const char *ptr = token.pointer;
        const char *endPtr = token.pointer + token.len;

        std::cout << std::endl << reinterpret_cast<const void *>(ptr) << " " << token.kind << std::endl;

        while (ptr < endPtr) {
            std::cout << *ptr;
            ++ptr;
        }

    } while (token.kind != renderbox::eof);

}
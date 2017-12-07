
#include <iostream>
#include "renderbox.h"

int main(int argc, char** argv) {

    renderbox::GLSLPreprocessor preprocessor;
    renderbox::GLSLToken token;

    preprocessor.enterMainSourceFile("shaders/shader.glsl");
    do {
        preprocessor.lex(token);

        std::cout << "kind" << " " << token.kind << std::endl
                  << token << std::endl;

    } while (token.kind != renderbox::eof);

}

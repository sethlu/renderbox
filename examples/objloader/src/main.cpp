#include <iostream>
#include "renderbox.h"

int main(int argc, char **argv) {

    std::string obj = renderbox::readFile("scene.obj");

    renderbox::OBJLexer lexer(obj.c_str(), obj.c_str());
    renderbox::OBJToken token{};

    do {

        lexer.lex(token);
        std::cout << " " << token;

    } while (token.kind != renderbox::obj_tok::eof);

}
#include <iostream>
#include "renderbox.h"

int main(int argc, char **argv) {

    auto scene = std::make_shared<renderbox::Scene>();

    std::string obj = renderbox::readFile("scene.obj");
    renderbox::OBJLoader loader(scene);
    loader.loadOBJ(obj.c_str());

}
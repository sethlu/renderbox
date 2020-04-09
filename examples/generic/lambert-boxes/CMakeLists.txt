
cmake_minimum_required(VERSION 3.7)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_CXX_STANDARD 11)


file(GLOB_RECURSE SOURCE_FILES src/*.cpp)

add_executable(example-lambert-boxes ${SOURCE_FILES})
target_link_libraries(example-lambert-boxes renderbox)

install(TARGETS example-lambert-boxes DESTINATION bin)

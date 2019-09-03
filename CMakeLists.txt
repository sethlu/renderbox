
cmake_minimum_required(VERSION 3.7)
project(renderbox)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_CXX_STANDARD 14)


# OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})

# GLM
add_subdirectory(vendor/glm)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(vendor/glfw)

# SDL
add_subdirectory(vendor/sdl)


# RenderBox

file(GLOB_RECURSE RENDERBOX_SOURCES src/*.cpp src/*.mm)
file(GLOB_RECURSE RENDERBOX_HEADERS include/*.h)

function (REMOVE_SOURCES)
    file(GLOB_RECURSE TO_REMOVE ${ARGN})
    list(REMOVE_ITEM RENDERBOX_SOURCES ${TO_REMOVE})
    set(RENDERBOX_SOURCES ${RENDERBOX_SOURCES} PARENT_SCOPE)
endfunction()

if(NOT APPLE)
    REMOVE_SOURCES(src/renderers/metal/platforms/apple/** src/renderers/opengl/platforms/apple/**)
endif()

if (NOT UNIX OR APPLE)
    REMOVE_SOURCES(src/renderers/opengl/platforms/linux/**)
endif()

add_library(renderbox ${RENDERBOX_SOURCES} ${RENDERBOX_HEADERS})
target_include_directories(renderbox
        PUBLIC include
        PUBLIC vendor/glfw/include
        PUBLIC vendor/sdl/include)
target_link_libraries(renderbox ${OPENGL_LIBRARIES} glm glfw SDL2-static)

install(TARGETS renderbox DESTINATION lib)
install(FILES ${RENDERBOX_HEADERS} DESTINATION include/renderbox)


# Examples
add_subdirectory(examples)

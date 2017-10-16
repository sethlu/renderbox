cmake_minimum_required(VERSION 3.7)
project(vox)


# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(vendor/glfw)


set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(CMAKE_CXX_STANDARD 11)


# RenderBox
file(GLOB_RECURSE RENDERBOX_SOURCE_FILES renderbox/*.cpp renderbox/*.h)

set(SOURCE_FILES src/main.cpp)
add_executable(vox ${RENDERBOX_SOURCE_FILES} ${SOURCE_FILES})

# OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})

# GLM
find_package(GLM REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})


target_link_libraries(vox glfw ${OPENGL_LIBRARIES} ${GLM_LIBRARY})
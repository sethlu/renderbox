cmake_minimum_required(VERSION 3.7)
project(particlesim)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(CMAKE_CXX_STANDARD 11)

# RenderBox
file(GLOB_RECURSE RENDERBOX_SOURCE_FILES src/renderbox/*.cpp src/renderbox/*.h)

set(SOURCE_FILES src/main.cpp)
add_executable(particlesim ${RENDERBOX_SOURCE_FILES} ${SOURCE_FILES})

# OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})

# GLEW
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

# GLFW
find_package(GLFW REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS})

# GLM
find_package(GLM REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})


target_link_libraries(particlesim ${OPENGL_LIBRARIES} ${GLEW_LIBRARY} ${GLFW_LIBRARY} ${GLM_LIBRARY})
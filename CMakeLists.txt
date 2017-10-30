
cmake_minimum_required(VERSION 3.7)
project(renderbox)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_CXX_STANDARD 11)


# OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})

# GLM
find_package(GLM REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(vendor/glfw)


file(GLOB_RECURSE RENDERBOX_SOURCE_FILES src/*.cpp)
file(GLOB_RECURSE RENDERBOX_HEADER_FILES include/*.h)

add_library(renderbox ${RENDERBOX_SOURCE_FILES} ${RENDERBOX_HEADER_FILES})
target_link_libraries(renderbox ${OPENGL_LIBRARIES} ${GLM_LIBRARY} glfw)
target_include_directories(renderbox PUBLIC include)

install(TARGETS renderbox DESTINATION lib)
install(FILES ${RENDERBOX_HEADER_FILES} DESTINATION include/renderbox)


# Examples
add_subdirectory(examples)

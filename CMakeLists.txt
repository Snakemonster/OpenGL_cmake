cmake_minimum_required(VERSION 3.21)
project(OpenGL_cmake)

set(CMAKE_CXX_STANDARD 20)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

include_directories(third-party)
include_directories(third-party/glad/include)

find_package(OpenGL)
include_directories(third-party/glfw3/include)
add_subdirectory(third-party/glfw3)

if(WIN32)
    add_subdirectory(third-party/glm)
else()
    if (UNIX AND NOT APPLE)
        find_package(glm)
    endif ()
endif()

add_executable(OpenGL_cmake main.cpp third-party/glad/src/glad.c)
if (WIN32)
    target_link_libraries(OpenGL_cmake glm)
endif ()
target_link_libraries(OpenGL_cmake glfw)
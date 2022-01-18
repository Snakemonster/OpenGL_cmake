cmake_minimum_required(VERSION 3.21)
project(OpenGL_cmake)

set(CMAKE_CXX_STANDARD 20)

#-------------------------------------------------
#setting up libs
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
#end of setting up libs
#-------------------------------------------------


#-------------------------------------------------
#setting up directories
set(dir ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${dir}/build)
file(COPY resources DESTINATION ${dir}/build)
file(COPY shaders DESTINATION ${dir}/build)

file (GLOB_RECURSE SRC_OPENGL_CMAKE CONFIGURE_DEPENDS "src/*.cpp" "third-party/glad/src/glad.c")
file (GLOB_RECURSE CUSTOM_HEADERS CONFIGURE_DEPENDS "includes/*.h")

set (INCLUDE_DIRS_OPENGL_CMAKE "")
foreach (_headerFile ${CUSTOM_HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND INCLUDE_DIRS_OPENGL_CMAKE ${_dir})
endforeach()
list (REMOVE_DUPLICATES INCLUDE_DIRS_OPENGL_CMAKE)
#end of setting up directories
#-------------------------------------------------

add_executable(OpenGL_cmake ${SRC_OPENGL_CMAKE})
target_include_directories(OpenGL_cmake PRIVATE ${INCLUDE_DIRS_OPENGL_CMAKE})

if (WIN32)
    target_link_libraries(OpenGL_cmake glm)
endif ()
target_link_libraries(OpenGL_cmake glfw)
#
# Healpix viewer program
#

cmake_minimum_required(VERSION 3.10)

project(healpixviewer)

# morphologica uses c++-20 language features
set(CMAKE_CXX_STANDARD 20)

# Add the host definition to CXXFLAGS along with other switches, depending on OS/Compiler
if (APPLE)
  set(CMAKE_CXX_FLAGS "-Wall -Wfatal-errors -g -O3")
else()
  # This assumes a gcc compiler (or a gcc mimic like Clang)
  set(COMPREHENSIVE_WARNING_FLAGS "-Wall -Wextra -Wpedantic -pedantic-errors -Werror -Wfatal-errors -Wno-psabi -Wno-unknown-pragmas")
  set(CMAKE_CXX_FLAGS "-g -O3 ${COMPREHENSIVE_WARNING_FLAGS}")
endif()

if(APPLE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGL_SILENCE_DEPRECATION")
endif()

# Tell the program where the morph fonts are, to compile them into the binary
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMORPH_FONTS_DIR=\"\\\"${PROJECT_SOURCE_DIR}/morphologica/fonts\\\"\"")

# Find the libraries which will be needed by morphologica
set(OpenGL_GL_PREFERENCE "GLVND") # Following `cmake --help-policy CMP0072`
find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(Freetype REQUIRED)
# Note: chealpix is also required! E.g. from apt install libchealpix-dev on Debian/Ubuntu

# include files for graphics
include_directories(${OPENGL_INCLUDE_DIR} ${GLFW3_INCLUDE_DIR} ${FREETYPE_INCLUDE_DIRS})
# Assuming that you installed morphologica in-tree (i.e. 'next to' yourprog.cpp).
set(MORPH_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/morphologica" CACHE PATH "The path to morphologica")
include_directories(BEFORE ${MORPH_INCLUDE_PATH}/include) # Allows GL3/gl3.h to be found
include_directories(BEFORE ${MORPH_INCLUDE_PATH})         # Allows morph/Header.h to be found

#
# All the library finding is now done. It's time to compile the program.
#

# The viewer is compiled from a single c++ file
add_executable(viewer viewer.cpp)
target_link_libraries(viewer OpenGL::GL Freetype::Freetype glfw chealpix)

#
# All done!
#

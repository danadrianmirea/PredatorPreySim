cmake_minimum_required(VERSION 3.15)
project(PredatorPreySim VERSION 1.0)

# Set C++20 for modern features
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Statically link GCC and C++ runtimes
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++ -static")

# Define where project headers are
include_directories(include)

# Raylib configuration - use the local prebuilt package
set(RAYLIB_DIR "${CMAKE_SOURCE_DIR}/raylib-5.5_win64_mingw-w64")
include_directories(${RAYLIB_DIR}/include)
link_directories(${RAYLIB_DIR}/lib)

# Find all .cpp files in src/ and its subdirectories
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link raylib static library and system libraries needed on Windows
target_link_libraries(${PROJECT_NAME} raylib winmm gdi32 opengl32)

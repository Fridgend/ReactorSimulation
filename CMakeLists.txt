cmake_minimum_required(VERSION 3.10)
project(reactor)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)

# Source files
file(GLOB SOURCES "src/*.cpp")

# Add executable
add_executable(reactor ${SOURCES})
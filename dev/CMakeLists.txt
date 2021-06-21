cmake_minimum_required(VERSION 3.10)

# Generate file for clangd LSP
set(CMAKE_EXPORT_COMPILE_COMMANDS true)

project(TesteGB)

# All source files
file(GLOB SRC_FILES src/*.cpp)
file(GLOB SRC_FILES_C src/*.c)

add_executable(app ${SRC_FILES} ${SRC_FILES_C})

# C++11 standard
set_target_properties(app PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED TRUE
    CXX_EXTENSIONS TRUE
)

find_package(glfw3 3.3.4 REQUIRED)
target_link_libraries(app glfw)

include_directories("include")

cmake_minimum_required(VERSION 3.10)

# Generate file for clangd LSP
set(CMAKE_EXPORT_COMPILE_COMMANDS true)

project(TesteGB)

# All source files
file(GLOB SRC_FILES src/*.cpp)
file(GLOB SRC_FILES_C src/*.c)

add_executable(app ${SRC_FILES} ${SRC_FILES_C})

find_package(glfw3 3.3.4 REQUIRED)
target_link_libraries(app glfw)

include_directories("include")

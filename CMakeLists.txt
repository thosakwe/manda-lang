cmake_minimum_required(VERSION 3.0)
project(manda)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
list(APPEND CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_LIST_DIR}/free-cmake-scripts/find_package")
add_subdirectory(src)

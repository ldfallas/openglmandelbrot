cmake_minimum_required(VERSION 3.0.0)
project(myshaderexperiment VERSION 0.1.0)

include(CTest)
enable_testing()


list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake")

find_package(glfw3 REQUIRED)

add_executable(myshaderexperiment main.cpp glad/glad.c)

target_link_libraries(myshaderexperiment PRIVATE glfw3)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

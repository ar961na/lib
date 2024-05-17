cmake_minimum_required(VERSION 3.27)
project(radar2)

set(CMAKE_CXX_STANDARD 20)

add_executable(radar2 main.cpp)

include(FetchContent)

FetchContent_Declare(
        glm
        GIT_REPOSITORY	https://github.com/g-truc/glm.git
        GIT_TAG 	bf71a834948186f4097caa076cd2663c69a10e1e #refs/tags/0.9.9.8
)

FetchContent_MakeAvailable(glm)

find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
target_link_libraries(radar2 INTERFACE
        Python3::Python
        Python3::Module
)

target_link_libraries(radar2 PRIVATE glm::glm)

cmake_minimum_required(VERSION 3.26)
project(test_sfml)

set(CMAKE_CXX_STANDARD 23)

include(FetchContent)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG        2.6.1
)
FetchContent_MakeAvailable(SFML)

add_executable(test_sfml main.cpp)
target_link_libraries(test_sfml PRIVATE sfml-graphics sfml-window sfml-system)

cmake_minimum_required(VERSION 3.14)
project(lingebra)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include(FetchContent)
FetchContent_Declare(
    unitgrad
    GIT_REPOSITORY  https://github.com/Lebogang-G-Masia/unitgrad.git
    GIT_TAG         main
)
FetchContent_MakeAvailable(unitgrad)
add_library(lingebra INTERFACE)
target_include_directories(lingebra INTERFACE unitgrad)
target_link_libraries(lingebra INTERFACE unitgrad)
add_compile_options(-g)
add_executable(test src/test.cpp)
target_link_libraries(test PRIVATE lingebra)

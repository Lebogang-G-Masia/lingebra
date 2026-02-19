cmake_minimum_required(VERSION 3.14)
project(lingebra_project)

add_library(lingebra 
    include/matrix.hpp 
    include/tensor.hpp
)

target_include_directories(lingebra PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

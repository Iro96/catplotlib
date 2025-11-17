cmake_minimum_required(VERSION 3.15)
project(catplot VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(CATPLOT_BUILD_EXAMPLES "Build catplot examples" ON)

add_library(catplot
    src/figure.cpp
    src/axes.cpp
    src/svg_canvas.cpp
    src/svg_backend.cpp
)
target_include_directories(catplot PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

if(CATPLOT_BUILD_EXAMPLES)
    add_executable(catplot_example examples/simple.cpp)
    target_link_libraries(catplot_example PRIVATE catplot)
endif()



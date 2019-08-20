message("Added sub direcotory ${CMAKE_CURRENT_SOURCE_DIR}")

add_library(CNNet STATIC
    Activation.cpp 
    Activation.hpp
    CNN.cpp
    CNN.hpp
    ConvolutionLayer.cpp
    ConvolutionLayer.hpp
    FCLayer.cpp
    FCLayer.hpp
    PoolLayer.cpp
    PoolLayer.hpp)   


target_include_directories(CNNet PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Eigen")
target_include_directories(CNNet PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
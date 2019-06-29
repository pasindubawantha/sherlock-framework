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

message("Include directory CNNnet : ${CMAKE_CURRENT_SOURCE_DIR}/.., ${CMAKE_CURRENT_SOURCE_DIR}/Eigen")
target_include_directories(CNNet PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Eigen")
target_link_libraries(CNNet PUBLIC logger)
# target_link_libraries(CNNet PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Eigen")
target_include_directories(CNNet PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/..")
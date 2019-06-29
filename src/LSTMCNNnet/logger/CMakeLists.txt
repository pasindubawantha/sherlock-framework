message("Added sub direcotory ${CMAKE_CURRENT_SOURCE_DIR}")
add_library(logger STATIC
    logger.cpp
    logger.hpp)   

target_include_directories(logger PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
message("Added sub direcotory ${CMAKE_CURRENT_SOURCE_DIR}")

add_library(LSTMnet STATIC
    DataProcessor.cpp
    DataProcessor.h
    FileProcessor.cpp
    FileProcessor.h
    LSTMNet.cpp
    LSTMNet.h)   

target_link_libraries(LSTMnet PUBLIC CNNet)


target_include_directories(LSTMnet PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
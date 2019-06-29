message("Added sub direcotory ${CMAKE_CURRENT_SOURCE_DIR}")
add_library(LSTMCNnet STATIC
    CNNLSTMPredictionModel.cpp
    CNNLSTMPredictionModel.hpp
    CNNPredictionModel.cpp
    CNNPredictionModel.hpp
    DTW.cpp
    DTW.hpp
    LSTMCNnet.hpp
    LSTMCNNFCPredictionModel.cpp
    LSTMCNNFCPredictionModel.hpp
    LSTMCNNPredictionModel.cpp
    LSTMCNNPredictionModel.hpp
    LSTMPredictionModel.cpp
    LSTMPredictionModel.hpp
    PredictionModel.cpp
    PredictionModel.hpp)

target_link_libraries(LSTMCNnet PUBLIC LSTMnet)
target_link_libraries(LSTMCNnet PUBLIC CNNet)
target_link_libraries(LSTMCNnet PUBLIC logger)

message("Include directory LSTMCNnet : ${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(LSTMCNnet PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
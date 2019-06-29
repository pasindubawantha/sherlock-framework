message("Added sub direcotory ${CMAKE_CURRENT_SOURCE_DIR}")
add_subdirectory(LSTMCNNnet)


add_executable(Sherlock main.cpp)   

target_link_libraries(Sherlock PUBLIC LSTMCNnet)
target_link_libraries(Sherlock PUBLIC LSTMnet)
target_link_libraries(Sherlock PUBLIC CNNet)
target_link_libraries(Sherlock PUBLIC logger)

message("Include directory : ${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(Sherlock PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")


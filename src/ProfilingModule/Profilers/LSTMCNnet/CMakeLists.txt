message("Added sub direcotory ${CMAKE_CURRENT_SOURCE_DIR}")

add_subdirectory(CNNet) #add libs first 
add_subdirectory(LSTMnet)
add_subdirectory(LSTMCNnet)
message("Added sub direcotory ${CMAKE_CURRENT_SOURCE_DIR}")
# add_subdirectory(LSTMCNNnet)

# add_executable(Sherlock main.cpp MainLoop.cpp HistoryBuffer.cpp MutexLock.cpp) 
add_executable(Sherlock main.cpp MainLoop.cpp HistoryBuffer.cpp) 

target_include_directories(Sherlock PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")  
add_subdirectory(ProfilingModule)

# add_executable(Sherlock main.cpp MainLoop.cpp HistoryBuffer.cpp MutexLock.cpp)   

# target_link_libraries(Sherlock PUBLIC LSTMCNnet)
# target_link_libraries(Sherlock PUBLIC LSTMnet)
# target_link_libraries(Sherlock PUBLIC CNNet)
# target_link_libraries(Sherlock PUBLIC logger)

target_include_directories(Sherlock PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(Sherlock PUBLIC Profilers)

# message("Include directory : ${CMAKE_CURRENT_SOURCE_DIR}/include, ${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(Sherlock PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(Sherlock PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
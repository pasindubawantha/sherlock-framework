message("Added sub direcotory ${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(Sherlock main.cpp MainLoop.cpp HistoryBuffer.cpp MutexLock.cpp) 


add_subdirectory(ProfilingModule)
add_subdirectory(AnomalyDetectionModule)

target_link_libraries(Sherlock PUBLIC Profilers)
target_link_libraries(Sherlock PUBLIC AnomalyDetectors)


target_include_directories(Sherlock PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
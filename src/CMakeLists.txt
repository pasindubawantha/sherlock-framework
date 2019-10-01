message("Added sub direcotory ${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(Sherlock main.cpp MainLoop.cpp HistoryBuffer.cpp MutexLock.cpp) 
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(ProfilingModule)
add_subdirectory(AnomalyDetectionModule)
add_subdirectory(ConceptDriftDectectionModule)

target_link_libraries(Sherlock PUBLIC Profilers)
target_link_libraries(Sherlock PUBLIC AnomalyDetectors)
target_link_libraries(Sherlock PUBLIC ConceptDriftDetectors)

target_link_libraries(Sherlock PUBLIC stdc++fs)

target_include_directories(Sherlock PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(Sherlock PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Includes")
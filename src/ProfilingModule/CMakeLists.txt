message("Added sub direcotory ${CMAKE_CURRENT_SOURCE_DIR}")

add_library(Profilers STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Profilers/Profiler.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Profilers/default/DefaultProfiler.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Profilers/default/DefaultProfiler.cpp
   )

message("Include directory LSTMProfilersnet : ${CMAKE_CURRENT_SOURCE_DIR}/Profilers")

target_include_directories(Profilers PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Profilers")
target_include_directories(Profilers PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Profilers/default")
# target_include_directories(Profilers PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Profilers/default")
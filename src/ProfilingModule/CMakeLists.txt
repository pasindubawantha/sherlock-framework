message("Added sub direcotory ${CMAKE_CURRENT_SOURCE_DIR}")


add_library(Profilers STATIC
    # Add files profilers
    ${CMAKE_CURRENT_SOURCE_DIR}/Profilers/Profiler.h
    
    ${CMAKE_CURRENT_SOURCE_DIR}/Profilers/default/DefaultProfiler.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Profilers/default/DefaultProfiler.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/Profilers/LSTMCNnet/LSTMCNnetProfiler.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Profilers/LSTMCNnet/LSTMCNnetProfiler.cpp
   
    )

target_include_directories(Profilers PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../")

# Include Prodilers 
target_include_directories(Profilers PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Profilers")
target_include_directories(Profilers PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Profilers/default")
target_include_directories(Profilers PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Profilers/LSTMCNnet")



# Add subdirectories of prodilers if needed
add_subdirectory(Profilers/LSTMCNnet) # For LSTMCNnet

# Link Profiler libraries if needed 
target_link_libraries(Profilers PUBLIC LSTMCNnet) # For LSTMCNnet




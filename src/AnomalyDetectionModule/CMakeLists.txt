message("Added sub direcotory ${CMAKE_CURRENT_SOURCE_DIR}")



add_library(AnomalyDetectors STATIC
    
    # Add files Anomaly Detectors
    ${CMAKE_CURRENT_SOURCE_DIR}/AnomalyDetectors/AnomalyDetector.h

    ${CMAKE_CURRENT_SOURCE_DIR}/AnomalyDetectors/default/AnomalyDetector.h
    ${CMAKE_CURRENT_SOURCE_DIR}/AnomalyDetectors/default/AnomalyDetector.cpp
    
    # Add files Anomaly Anomaly Distance Measures
    ${CMAKE_CURRENT_SOURCE_DIR}/AnomalyDistanceMeasures/AnomalyDistanceMeasure.h

    # Add files Anomaly Threshold Setters
    ${CMAKE_CURRENT_SOURCE_DIR}/AnomalyThresholdSetters/AnomalyThresholdSetter.h

   )

target_include_directories(Profilers PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../")

# Include Anomaly Detectors
target_include_directories(Profilers PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/AnomalyDetectors")


# Include Anomaly Distance Measures
target_include_directories(Profilers PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/AnomalyDistanceMeasures")


# Include Anomaly Threshold Setters
target_include_directories(Profilers PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/AnomalyThresholdSetters")




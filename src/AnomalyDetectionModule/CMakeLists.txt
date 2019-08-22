message("Added sub direcotory ${CMAKE_CURRENT_SOURCE_DIR}")



add_library(AnomalyDetectors STATIC
    
    # Add files Anomaly Detectors
    ${CMAKE_CURRENT_SOURCE_DIR}/AnomalyDetectors/AnomalyDetector.h

    ${CMAKE_CURRENT_SOURCE_DIR}/AnomalyDetectors/default/DefaultAnomalyDetector.h
    ${CMAKE_CURRENT_SOURCE_DIR}/AnomalyDetectors/default/DefaultAnomalyDetector.cpp
    
    # Add files Anomaly Anomaly Distance Measures
    ${CMAKE_CURRENT_SOURCE_DIR}/AnomalyDistanceMeasures/AnomalyDistanceMeasure.h

    ${CMAKE_CURRENT_SOURCE_DIR}/AnomalyDistanceMeasures/FastDTW/FastDTWAnomalyDistanceMeasure.h
    ${CMAKE_CURRENT_SOURCE_DIR}/AnomalyDistanceMeasures/FastDTW/FastDTWAnomalyDistanceMeasure.cpp

    # Add files Anomaly Threshold Setters
    ${CMAKE_CURRENT_SOURCE_DIR}/AnomalyThresholdSetters/AnomalyThresholdSetter.h
    
    ${CMAKE_CURRENT_SOURCE_DIR}/AnomalyThresholdSetters/TrainingMax/TrainingMaxAnomalyThresholdSetter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/AnomalyThresholdSetters/TrainingMax/TrainingMaxAnomalyThresholdSetter.cpp

   )

target_include_directories(AnomalyDetectors PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../")

# Include Anomaly Detectors
target_include_directories(AnomalyDetectors PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/AnomalyDetectors")
target_include_directories(AnomalyDetectors PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/AnomalyDetectors/default")



# Include Anomaly Distance Measures
target_include_directories(AnomalyDetectors PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/AnomalyDistanceMeasures")
target_include_directories(AnomalyDetectors PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/AnomalyDistanceMeasures/FastDTW")
target_include_directories(AnomalyDetectors PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/AnomalyDistanceMeasures/FastDTW/Eigen")

# Include Anomaly Threshold Setters
target_include_directories(AnomalyDetectors PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/AnomalyThresholdSetters")
target_include_directories(AnomalyDetectors PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/AnomalyThresholdSetters/TrainingMax")






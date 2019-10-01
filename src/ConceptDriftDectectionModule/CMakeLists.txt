message("Added sub direcotory ${CMAKE_CURRENT_SOURCE_DIR}")



add_library(ConceptDriftDetectors STATIC
    
    # Add files Concept Detectors
    ${CMAKE_CURRENT_SOURCE_DIR}/ConceptDriftDetectors/ConceptDriftDetector.h

    ${CMAKE_CURRENT_SOURCE_DIR}/ConceptDriftDetectors/DynamicWindow/DynamicWindowConceptDriftDetector.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ConceptDriftDetectors/DynamicWindow/DynamicWindowConceptDriftDetector.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/ConceptDriftDetectors/NoConceptDriftDetector/NoConceptDriftDetector.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ConceptDriftDetectors/NoConceptDriftDetector/NoConceptDriftDetector.cpp
    
    # Add files Concept Distance Measures
    ${CMAKE_CURRENT_SOURCE_DIR}/ConceptDistanceMeasures/ConceptDistanceMeasure.h

    ${CMAKE_CURRENT_SOURCE_DIR}/ConceptDistanceMeasures/FastDTW/FastDTWConceptDistanceMeasure.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ConceptDistanceMeasures/FastDTW/FastDTWConceptDistanceMeasure.cpp

    # Add files Concept Threshold Setters
    ${CMAKE_CURRENT_SOURCE_DIR}/ConceptThresholdSetters/ConceptThresholdSetter.h
    
    ${CMAKE_CURRENT_SOURCE_DIR}/ConceptThresholdSetters/ConfidenceInterval/ConfidenceIntervalConceptThresholdSetter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ConceptThresholdSetters/ConfidenceInterval/ConfidenceIntervalConceptThresholdSetter.cpp

   )

target_include_directories(ConceptDriftDetectors PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../")

# # Include Concept Drift Detectors
target_include_directories(ConceptDriftDetectors PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/ConceptDriftDetectors")
target_include_directories(ConceptDriftDetectors PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/ConceptDriftDetectors/DynamicWindow")
target_include_directories(ConceptDriftDetectors PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/ConceptDriftDetectors/NoConceptDriftDetector")


# Include Concept Distance Measures
target_include_directories(ConceptDriftDetectors PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/ConceptDistanceMeasures")
target_include_directories(ConceptDriftDetectors PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/ConceptDistanceMeasures/FastDTW")
target_include_directories(ConceptDriftDetectors PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/ConceptDistanceMeasures/FastDTW/Eigen")

# # Include Concept Threshold Setters
target_include_directories(ConceptDriftDetectors PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/ConceptThresholdSetters")
target_include_directories(ConceptDriftDetectors PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/ConceptThresholdSetters/ConfidenceInterval")






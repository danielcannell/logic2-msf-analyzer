cmake_minimum_required (VERSION 3.13)

project(MSFAnalyzer LANGUAGES CXX)

add_definitions(-DLOGIC2)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

include(ExternalAnalyzerSDK)

add_analyzer_plugin(${PROJECT_NAME} SOURCES
    src/MSFAnalyzer.cpp
    src/MSFAnalyzer.h
    src/MSFAnalyzerResults.cpp
    src/MSFAnalyzerResults.h
    src/MSFAnalyzerSettings.cpp
    src/MSFAnalyzerSettings.h
    src/MSFSimulationDataGenerator.cpp
    src/MSFSimulationDataGenerator.h
)

cmake_minimum_required(VERSION 3.19)

# PATH OF THE MAX SDK
set (C74_SDK "" CACHE PATH "Path to the Max SDK")
if(NOT C74_SDK)
  message(FATAL_ERROR "Max SDK Path is not set")
endif()

string(REGEX REPLACE "(.*)/" "" THIS_FOLDER_NAME "${CMAKE_CURRENT_SOURCE_DIR}")
project(${THIS_FOLDER_NAME})

set (C74_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../externals)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/mc.bufGranul~)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/liveGranul~)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/synGranul~)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/rand_dist_list)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/rand_dist_list~)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/idleubutton)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/bargraf~)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/mc.synGranul~)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/mc.liveGranul~)
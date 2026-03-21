cmake_minimum_required(VERSION 3.10)
project(electrode_brain_devs)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

if(NOT DEFINED ENV{CADMIUM})
  message(FATAL_ERROR "Environment variable CADMIUM is not set. Please set CADMIUM to your Cadmium v2 root directory.")
endif()

set(CADMIUM_ROOT $ENV{CADMIUM})

include_directories(${CADMIUM_ROOT})

add_subdirectory(main)

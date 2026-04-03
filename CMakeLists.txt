cmake_minimum_required(VERSION 3.15)

project(Re1HdDoorSkip LANGUAGES CXX)

if (NOT MSVC)
    message(FATAL_ERROR "This project must be built using MSVC due to MSVC-specific linker options.")
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type")
endif()

add_library(Re1HdDoorSkip SHARED DllMain.cpp UltimateProxyDLL.h ModStuff.h)
target_compile_features(Re1HdDoorSkip PRIVATE cxx_std_17)
set_target_properties(Re1HdDoorSkip PROPERTIES LINKER_LANGUAGE CXX OUTPUT_NAME dinput8)
set_property(TARGET Re1HdDoorSkip PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
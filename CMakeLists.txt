cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(gooscript VERSION 1.0)

# file(GLOB_RECURSE SOURCE_FILES "src/*.cpp" "src/*.hpp")
set(SOURCE_FILES 
  src/main.cpp
  src/token.cpp
)
set(LIBS_DIR "${PROJECT_SOURCE_DIR}/libs")

# Executable
include_directories(${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/src ${LIBS_DIR})
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
else()
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif()

target_compile_definitions(${PROJECT_NAME}
                           PRIVATE "DEBUG=$<IF:$<CONFIG:Debug>,1,0>")

add_subdirectory("libs/crabpp")
include_directories(${PROJECT_NAME} ${LIBS_DIR}/crabpp/inc)

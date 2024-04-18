cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

set(CMAKE_CXX_FLAGS 
  "-fexceptions \
  -Werror \
  -Wall \
  -Wextra \
  -Wpedantic \
  -Wuninitialized \
  -Wshadow \
  -Wundef \
  -Winvalid-pch\
  -Wswitch-enum\
  -Wswitch-default\
  -Wfloat-equal\
  -Wcast-qual\
  -Wconversion\
  -Wpacked\
  -Wstrict-aliasing\
  -Wmissing-declarations\
  -Wmissing-field-initializers\
  -Wold-style-cast\
  -Woverloaded-virtual\
  -Wsign-compare\
")




project(gooscript VERSION 1.0)


# file(GLOB_RECURSE SOURCE_FILES "src/*.cpp" "src/*.hpp")
set(SOURCE_FILES 
  src/main.cpp
        src/lexer/Token.cpp
        src/lexer/Operator.cpp
)
set(LIBS_DIR "${PROJECT_SOURCE_DIR}/libs")

# Executable
include_directories(${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/src ${LIBS_DIR})

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
add_compile_options("$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CXX_FLAGS}>")
# target_compile_options(${PROJECT_NAME} PRIVATE ${CMAKE_CXX_FLAGS})

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

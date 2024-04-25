cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(gooscript VERSION 1.0)

#file(GLOB_RECURSE SOURCE_FILES "src/*.hpp")
set(SOURCE_FILES
        src/main.cpp
        src/lexer/Keyword.cpp
        src/lexer/Operator.cpp
        src/token/Token.cpp
        src/token/Keyword.cpp
        src/token/Operator.cpp
        src/lexer/Lexer.cpp
        src/lexer/Lexer.hpp
)

# Executable
include_directories(${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/src)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
else ()
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif ()

target_compile_definitions(${PROJECT_NAME} PRIVATE
        "DEBUG=$<IF:$<CONFIG:Debug>,1,0>"
        "RELEASE=$<IF:$<CONFIG:Debug>,0,1>"
)


include(cmake/CPM.cmake)

CPMAddPackage("gh:bishan-batel/crab#main")
include_directories(${PROJECT_NAME} ${crabpp_SOURCE_DIR}/inc)

CPMAddPackage("gh:catchorg/Catch2@3.4.0")

target_link_libraries(${PROJECT_NAME} PRIVATE Catch2::Catch2WithMain crab)

#add_custom_target(pch COMMAND clang++ -std=gnu++23 -x c++-header
#        ${CMAKE_CURRENT_SOURCE_DIR}/libs/crabpp/inc/preamble.hpp
#        -o ${CMAKE_CURRENT_BINARY_DIR}/preamble.hpp.pch)

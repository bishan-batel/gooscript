cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
#set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(gooscript VERSION 1.0)

#set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 23)
#set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON) if(CMAKE_EXPORT_COMPILE_COMMANDS)
set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})


file(GLOB_RECURSE SOURCE_FILES "src/*.cpp" "src/*.hpp")
add_library(${PROJECT_NAME} ${SOURCE_FILES})


if (MSVC)
    add_compile_options(/Wall /WX /W2 /permissive-)
    add_link_options(/WX)
else ()
    # Clang and GCC
    add_compile_options(-std=c++23 -Wall -Wextra -Wpedantic -Werror)
endif ()
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

# add_executable(${PROJECT_NAME} ${SOURCE_FILES})

if (${CMAKE_BUILD_TYPE} STREQUAL "debug")
    add_compile_options(-g -O3)
else ()
    add_compile_options(-O3)
endif ()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

target_compile_definitions(${PROJECT_NAME} PRIVATE
        "DEBUG=$<IF:$<CONFIG:Debug>,1,0>"
        "RELEASE=$<IF:$<CONFIG:Debug>,0,1>"
)


include(cmake/CPM.cmake)

CPMAddPackage("gh:bishan-batel/crab#main")
CPMAddPackage("gh:fmtlib/fmt#10.2.1")


# Executable
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src ${crab_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME} PRIVATE crab fmt)

add_subdirectory(command)
add_subdirectory(test)
add_subdirectory(lsp)
#add_custom_target(pch COMMAND clang++ -std=gnu++23 -x c++-header
#        ${CMAKE_CURRENT_SOURCE_DIR}/libs/crabpp/inc/preamble.hpp
#        -o ${CMAKE_CURRENT_BINARY_DIR}/preamble.hpp.pch)


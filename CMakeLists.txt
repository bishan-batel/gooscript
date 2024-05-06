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
        src/token/Identifier.cpp
        src/token/Identifier.hpp
        src/token/Decimal.cpp
        src/token/Decimal.hpp
        src/token/Integer.cpp
        src/token/Integer.hpp
        src/token/StringLiteral.cpp
        src/token/StringLiteral.hpp
        src/ast/Expression.cpp
        src/ast/Expression.hpp
        src/ast/expression/Binary.cpp
        src/ast/expression/Binary.hpp
        src/ast/expression/Unary.cpp
        src/ast/expression/Unary.hpp
        src/ast/Statement.cpp
        src/ast/Statement.hpp
        src/meta/VariantType.hpp
        src/ast/expression/literal/Lambda.cpp
        src/ast/expression/literal/Lambda.hpp
        src/ast/expression/FunctionCall.cpp
        src/ast/expression/FunctionCall.hpp
        src/ast/expression/literal/StringLiteral.cpp
        src/ast/expression/literal/StringLiteral.hpp
        src/ast/expression/literal/Decimal.cpp
        src/ast/expression/literal/Decimal.hpp
        src/ast/expression/literal/Integer.cpp
        src/ast/expression/literal/Integer.hpp
        src/ast/expression/literal/Boolean.cpp
        src/ast/expression/literal/Boolean.hpp
        src/ast/expression/compound/ScopeBlock.cpp
        src/ast/expression/compound/ScopeBlock.hpp
        src/ast/statements/Eval.cpp
        src/ast/statements/Eval.hpp
        src/ast/expression/literal/Unit.cpp
        src/ast/expression/literal/Unit.hpp
        src/ast/expression/literal/Nil.cpp
        src/ast/expression/literal/Nil.hpp
        src/ast/statements/Return.cpp
        src/ast/statements/Return.hpp
        src/ast/expression/compound/If.cpp
        src/ast/expression/compound/If.hpp
        src/ast/expression/compound/While.cpp
        src/ast/expression/compound/While.hpp
        src/ast/expression/IdentifierBinding.cpp
        src/ast/expression/IdentifierBinding.hpp
        src/ast/expression/literal/Array.cpp
        src/ast/expression/literal/Array.hpp
        src/ast/expression/literal/Dictionary.cpp
        src/ast/expression/literal/Dictionary.hpp
        src/parser/TokenStream.cpp
        src/parser/TokenStream.hpp
        src/token/EndOfFile.cpp
        src/token/EndOfFile.hpp
        src/parser/Error.cpp
        src/parser/Error.hpp
        src/parser/pass/statement/block.cpp
        src/parser/pass/statement/block.hpp
        src/parser/pass/statement/statement.hpp
        src/parser/pass/statement/statement.cpp
        src/source/SourceFile.cpp
        src/source/SourceFile.hpp
        src/source/IoError.cpp
        src/source/IoError.hpp
)


add_library(${PROJECT_NAME} ${SOURCE_FILES})
# add_executable(${PROJECT_NAME} ${SOURCE_FILES})

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
CPMAddPackage("gh:fmtlib/fmt#10.2.1")


# Executable
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src ${crab_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME} PRIVATE crab fmt)
add_subdirectory(test)

#add_custom_target(pch COMMAND clang++ -std=gnu++23 -x c++-header
#        ${CMAKE_CURRENT_SOURCE_DIR}/libs/crabpp/inc/preamble.hpp
#        -o ${CMAKE_CURRENT_BINARY_DIR}/preamble.hpp.pch)

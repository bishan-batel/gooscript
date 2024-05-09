
CPMAddPackage("gh:catchorg/Catch2@3.5.4")

list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)

# Tests
add_executable(gooscript-tests
        lexer.cpp
        ast.cpp
)

target_link_libraries(gooscript-tests PRIVATE Catch2::Catch2WithMain crab fmt gooscript)

target_compile_definitions(gooscript-tests
        PRIVATE
        "DEBUG=$<IF:$<CONFIG:Debug>,1,0>"
)

include(CTest)
include(Catch)
catch_discover_tests(gooscript-tests)

CPMAddPackage("gh:catchorg/Catch2@3.5.4")

# Tests
add_executable(gooscript-tests
        lexer.cpp
)

target_link_libraries(gooscript-tests PRIVATE Catch2::Catch2WithMain crab fmt gooscript)

target_compile_definitions(gooscript-tests
        PRIVATE
        "DEBUG=$<IF:$<CONFIG:Debug>,1,0>"
)
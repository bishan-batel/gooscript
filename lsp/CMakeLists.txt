
# CPMAddPackage("gh:catchorg/Catch2@3.5.4")

# Tests
add_executable(gooscript-lsp
        main.cpp
        encoding.cpp
)

target_link_libraries(gooscript-lsp PRIVATE crab fmt gooscript)

target_compile_definitions(gooscript-lsp
        PRIVATE
        "DEBUG=$<IF:$<CONFIG:Debug>,1,0>")

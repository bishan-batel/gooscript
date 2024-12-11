
# CPMAddPackage("gh:catchorg/Catch2@3.5.4")

file(GLOB_RECURSE SOURCE_FILES "./*.cpp" "./*.hpp")

# Tests
add_executable(gooscript-lsp ${SOURCE_FILES})
target_include_directories(gooscript-lsp PUBLIC ./)

target_link_libraries(gooscript-lsp PRIVATE crab fmt gooscript)

target_compile_definitions(gooscript-lsp
        PRIVATE
        "DEBUG=$<IF:$<CONFIG:Debug>,1,0>")


# CPMAddPackage("gh:catchorg/Catch2@3.5.4")

# Tests
add_executable(gooscript-cmd
        main.cpp
)

target_link_libraries(gooscript-cmd PRIVATE crab fmt gooscript)

target_compile_definitions(gooscript-cmd
        PRIVATE
        "DEBUG=$<IF:$<CONFIG:Debug>,1,0>"
)

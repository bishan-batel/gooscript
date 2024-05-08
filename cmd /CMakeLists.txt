
# CPMAddPackage("gh:catchorg/Catch2@3.5.4")

# Tests
add_executable(goosey
        main.cpp
)

target_link_libraries(goosey PRIVATE crab fmt gooscript)

target_compile_definitions(goosey
        PRIVATE
        "DEBUG=$<IF:$<CONFIG:Debug>,1,0>"
)
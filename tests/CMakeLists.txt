# tests/CMakeLists.txt
# Test scaffold — wired for Catch2 (install separately) or any other framework.
# Enable with: cmake -B build -DBUILD_TESTS=ON
cmake_minimum_required(VERSION 3.21)

# ── Find Catch2 ───────────────────────────────────────────────────────────────
find_package(Catch2 QUIET)

if(NOT Catch2_FOUND)
    # Try alternative: check if Catch2 is installed via pkg-config or system
    message(STATUS "Trying to find Catch2...")
    find_package(Catch2 3 CONFIG QUIET)
endif()

if(Catch2_FOUND)
    message(STATUS "✓ Catch2 found — building tests")
    
    # Find SDL2 for linking
    if(WIN32)
        find_package(SDL2 CONFIG REQUIRED)
        find_package(SDL2_ttf CONFIG REQUIRED)
    else()
        find_package(SDL2 CONFIG QUIET)
        if(NOT SDL2_FOUND)
            list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
            find_package(SDL2 REQUIRED)
        endif()
        find_package(SDL2_ttf QUIET)
    endif()
    
    # Gather all source files needed for tests
    set(TEST_SOURCES
        test_bubble.cpp
        "${CMAKE_SOURCE_DIR}/src/classes/Bubble.cpp"
        "${CMAKE_SOURCE_DIR}/src/classes/BubbleGrid.cpp"
        "${CMAKE_SOURCE_DIR}/src/classes/Shooter.cpp"
        "${CMAKE_SOURCE_DIR}/src/classes/Score.cpp"
        "${CMAKE_SOURCE_DIR}/src/core/Renderer.cpp"
    )
    
    add_executable(BubbleTests ${TEST_SOURCES})
    target_link_libraries(BubbleTests PRIVATE Catch2::Catch2WithMain)
    target_link_libraries(BubbleTests PRIVATE SDL2::SDL2)
    
    # Only link SDL2_ttf if it's actually available
    if(SDL2_ttf_FOUND AND TARGET SDL2_ttf::SDL2_ttf)
        target_link_libraries(BubbleTests PRIVATE SDL2_ttf::SDL2_ttf)
        target_include_directories(BubbleTests PRIVATE ${SDL2_TTF_INCLUDE_DIRS})
    endif()
    
    target_include_directories(BubbleTests PRIVATE ${CMAKE_SOURCE_DIR}/src)
    
    # Compiler options
    target_compile_options(BubbleTests PRIVATE -Wall -Wextra -Wpedantic)
    
    # Register tests
    include(Catch)
    catch_discover_tests(BubbleTests VERBOSE)
    
    message(STATUS "✓ Test target 'BubbleTests' created")
else()
    message(WARNING "✗ Catch2 not found — test targets will be skipped")
    message(STATUS "   Install with: sudo apt install catch2 -y")
    message(STATUS "   Or with: vcpkg install catch2")
endif()

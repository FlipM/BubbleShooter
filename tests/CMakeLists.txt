# tests/CMakeLists.txt
# Test scaffold — wired for Catch2 (install separately) or any other framework.
# Enable with: cmake -B build -DBUILD_TESTS=ON
cmake_minimum_required(VERSION 3.21)

# ── Find Catch2 (optional) ────────────────────────────────────────────────────
find_package(Catch2 3 QUIET)

if(Catch2_FOUND)
    add_executable(BubbleTests
        test_bubble.cpp
    )
    target_link_libraries(BubbleTests PRIVATE Catch2::Catch2WithMain)
    target_include_directories(BubbleTests PRIVATE ${CMAKE_SOURCE_DIR}/src)
    include(Catch)
    catch_discover_tests(BubbleTests)
else()
    message(STATUS "Catch2 not found — skipping test targets. "
                   "Install with: sudo apt install catch2  OR  vcpkg install catch2")
endif()

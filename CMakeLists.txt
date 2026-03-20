cmake_minimum_required(VERSION 3.21)
project(BubbleShooter VERSION 0.1.0 LANGUAGES CXX)

# ── Standard ────────────────────────────────────────────────────────────────
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ── Output directories ───────────────────────────────────────────────────────
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# ── SDL2 discovery ───────────────────────────────────────────────────────────
# On Linux:  sudo apt install libsdl2-dev libsdl2-ttf-dev libsdl2-image-dev
# On Windows (vcpkg + MSVC): vcpkg install sdl2 sdl2-ttf sdl2-image --triplet=x64-windows
#            then: cmake -B build -DCMAKE_TOOLCHAIN_FILE=<vcpkg>/scripts/buildsystems/vcpkg.cmake
find_package(SDL2 CONFIG QUIET)
if(TARGET SDL2::SDL2)
    set(SDL2_LIBRARIES SDL2::SDL2)
else()
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
    find_package(SDL2 REQUIRED)
endif()

find_package(SDL2_ttf CONFIG QUIET)
find_package(SDL2_image CONFIG QUIET)

# ── Sources ──────────────────────────────────────────────────────────────────
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "src/*.cpp"
    "src/*.hpp"
)

add_executable(BubbleShooter ${SOURCES})

target_include_directories(BubbleShooter PRIVATE
    src
)

target_link_libraries(BubbleShooter PRIVATE
    $<IF:$<TARGET_EXISTS:SDL2::SDL2main>,SDL2::SDL2main,>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,${SDL2_LIBRARIES}>
)

if(SDL2_ttf_FOUND)
    target_include_directories(BubbleShooter PRIVATE ${SDL2_TTF_INCLUDE_DIRS})
    target_link_libraries(BubbleShooter PRIVATE SDL2_ttf::SDL2_ttf)
    target_compile_definitions(BubbleShooter PRIVATE HAS_SDL2_TTF)
endif()

if(SDL2_image_FOUND)
    target_include_directories(BubbleShooter PRIVATE ${SDL2_IMAGE_INCLUDE_DIRS})
    target_link_libraries(BubbleShooter PRIVATE SDL2_image::SDL2_image)
    target_compile_definitions(BubbleShooter PRIVATE HAS_SDL2_IMAGE)
endif()

# ── Windows-specific: avoid console window in Release ────────────────────────
if(WIN32)
    set_target_properties(BubbleShooter PROPERTIES
        WIN32_EXECUTABLE $<CONFIG:Release>
    )
endif()

# ── Compiler warnings ────────────────────────────────────────────────────────
if(MSVC)
    target_compile_options(BubbleShooter PRIVATE /W4 /permissive-)
else()
    target_compile_options(BubbleShooter PRIVATE -Wall -Wextra -Wpedantic)
endif()

# ── Tests ────────────────────────────────────────────────────────────────────
option(BUILD_TESTS "Build unit tests" OFF)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

message(STATUS "BubbleShooter configured — SDL2: ${SDL2_VERSION}")

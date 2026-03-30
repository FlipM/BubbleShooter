cmake_minimum_required(VERSION 3.21)

# Distribution-focused toggles (CMake-only, no source changes required)
option(BUBBLE_PREFER_STATIC_WINDOWS_VCPKG "Default to x64-windows-static when triplet is not explicitly set" ON)
option(BUBBLE_PREFER_STATIC_SDL "Prefer static SDL imported targets when available" ON)
option(BUBBLE_MSVC_STATIC_RUNTIME "Use static MSVC runtime (/MT, /MTd)" ON)
option(BUBBLE_REQUIRE_SDL2_TTF "Fail configuration if SDL2_ttf is missing" OFF)
option(BUBBLE_REQUIRE_SDL2_MIXER "Fail configuration if SDL2_mixer is missing" OFF)
option(BUBBLE_ENABLE_INSTALL "Enable install rules for distributable output" ON)
option(BUBBLE_ENABLE_CPACK "Enable CPack archive generation" ON)

# ── Windows zero-config bootstrap (vcpkg) ───────────────────────────────────
# Allows: cmake -B build
# without explicitly passing -DCMAKE_TOOLCHAIN_FILE or -DVCPKG_TARGET_TRIPLET.
if(WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(_VCPKG_CANDIDATES
        "C:/vcpkg/scripts/buildsystems/vcpkg.cmake"
    )

    if(DEFINED ENV{VCPKG_ROOT})
        list(PREPEND _VCPKG_CANDIDATES "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
    endif()

    foreach(_vcpkg_toolchain IN LISTS _VCPKG_CANDIDATES)
        if(EXISTS "${_vcpkg_toolchain}")
            set(CMAKE_TOOLCHAIN_FILE "${_vcpkg_toolchain}" CACHE FILEPATH "vcpkg toolchain file")
            break()
        endif()
    endforeach()

    if(NOT DEFINED VCPKG_TARGET_TRIPLET)
        if(BUBBLE_PREFER_STATIC_WINDOWS_VCPKG)
            set(VCPKG_TARGET_TRIPLET "x64-windows-static" CACHE STRING "vcpkg target triplet")
        else()
            set(VCPKG_TARGET_TRIPLET "x64-windows" CACHE STRING "vcpkg target triplet")
        endif()
    endif()
endif()

project(BubbleShooter VERSION 0.1.0 LANGUAGES CXX)

# ── Standard ────────────────────────────────────────────────────────────────
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC AND BUBBLE_MSVC_STATIC_RUNTIME)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# ── Output directories ───────────────────────────────────────────────────────
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# ── SDL2 discovery ───────────────────────────────────────────────────────────
# On Linux:  sudo apt install libsdl2-dev libsdl2-ttf-dev libsdl2-mixer-dev
# On Windows (vcpkg + MSVC): vcpkg install sdl2 sdl2-ttf sdl2-mixer --triplet=x64-windows
#            then: cmake -B build -DCMAKE_TOOLCHAIN_FILE=<vcpkg>/scripts/buildsystems/vcpkg.cmake

# On Windows, require vcpkg's CONFIG package to avoid accidental fallback
# to the custom FindSDL2.cmake module.
if(WIN32)
    find_package(SDL2 CONFIG REQUIRED)
else()
    # Non-Windows: try CONFIG mode first, then custom fallback module.
    find_package(SDL2 CONFIG QUIET)
    if(NOT SDL2_FOUND)
        list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
        find_package(SDL2 REQUIRED)
    endif()
endif()

if(TARGET SDL2::SDL2 OR TARGET SDL2::SDL2-static)
    if(BUBBLE_PREFER_STATIC_SDL AND TARGET SDL2::SDL2-static)
        set(SDL2_LIBRARIES SDL2::SDL2-static)
    elseif(TARGET SDL2::SDL2)
        set(SDL2_LIBRARIES SDL2::SDL2)
    else()
        set(SDL2_LIBRARIES SDL2::SDL2-static)
    endif()
endif()

if(BUBBLE_REQUIRE_SDL2_TTF)
    find_package(SDL2_ttf CONFIG REQUIRED)
else()
    find_package(SDL2_ttf CONFIG QUIET)
endif()

if(BUBBLE_REQUIRE_SDL2_MIXER)
    find_package(SDL2_mixer CONFIG REQUIRED)
else()
    find_package(SDL2_mixer CONFIG QUIET)
endif()

if(TARGET SDL2_ttf::SDL2_ttf OR TARGET SDL2_ttf::SDL2_ttf-static)
    if(BUBBLE_PREFER_STATIC_SDL AND TARGET SDL2_ttf::SDL2_ttf-static)
        set(BUBBLE_SDL2_TTF_TARGET SDL2_ttf::SDL2_ttf-static)
    elseif(TARGET SDL2_ttf::SDL2_ttf)
        set(BUBBLE_SDL2_TTF_TARGET SDL2_ttf::SDL2_ttf)
    else()
        set(BUBBLE_SDL2_TTF_TARGET SDL2_ttf::SDL2_ttf-static)
    endif()
endif()

if(TARGET SDL2_mixer::SDL2_mixer OR TARGET SDL2_mixer::SDL2_mixer-static)
    if(BUBBLE_PREFER_STATIC_SDL AND TARGET SDL2_mixer::SDL2_mixer-static)
        set(BUBBLE_SDL2_MIXER_TARGET SDL2_mixer::SDL2_mixer-static)
    elseif(TARGET SDL2_mixer::SDL2_mixer)
        set(BUBBLE_SDL2_MIXER_TARGET SDL2_mixer::SDL2_mixer)
    else()
        set(BUBBLE_SDL2_MIXER_TARGET SDL2_mixer::SDL2_mixer-static)
    endif()
endif()

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
    ${SDL2_LIBRARIES}
)

if(DEFINED BUBBLE_SDL2_TTF_TARGET)
    target_link_libraries(BubbleShooter PRIVATE ${BUBBLE_SDL2_TTF_TARGET})
endif()

if(DEFINED BUBBLE_SDL2_MIXER_TARGET)
    target_link_libraries(BubbleShooter PRIVATE ${BUBBLE_SDL2_MIXER_TARGET})
    target_compile_definitions(BubbleShooter PRIVATE HAS_SDL2_MIXER)
endif()

if(WIN32 AND BUBBLE_PREFER_STATIC_SDL)
    if(VCPKG_TARGET_TRIPLET AND NOT VCPKG_TARGET_TRIPLET MATCHES "-static$")
        message(WARNING "Static SDL requested, but VCPKG_TARGET_TRIPLET='${VCPKG_TARGET_TRIPLET}' is not static. Use -DVCPKG_TARGET_TRIPLET=x64-windows-static.")
    endif()
endif()

# ── Install/Package (portable output) ───────────────────────────────────────
if(BUBBLE_ENABLE_INSTALL)
    include(GNUInstallDirs)

    install(TARGETS BubbleShooter
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )

    if(EXISTS "${CMAKE_SOURCE_DIR}/assets")
        install(DIRECTORY "${CMAKE_SOURCE_DIR}/assets"
            DESTINATION .
        )
    endif()

    # If a dynamic build is used, copy runtime DLL dependencies as well.
    if(WIN32)
        install(FILES $<TARGET_RUNTIME_DLLS:BubbleShooter>
            DESTINATION ${CMAKE_INSTALL_BINDIR}
            OPTIONAL
        )
    endif()
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

if(BUBBLE_ENABLE_INSTALL AND BUBBLE_ENABLE_CPACK)
    set(CPACK_PACKAGE_NAME "BubbleShooter")
    set(CPACK_PACKAGE_VENDOR "BubbleShooter")
    set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
    set(CPACK_PACKAGE_DIRECTORY "${CMAKE_BINARY_DIR}/packages")
    set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY ON)

    if(WIN32)
        set(CPACK_GENERATOR "ZIP")
    else()
        set(CPACK_GENERATOR "TGZ")
    endif()

    include(CPack)
endif()

message(STATUS "BubbleShooter configured — SDL2: ${SDL2_VERSION}")
message(STATUS "BUBBLE_PREFER_STATIC_SDL=${BUBBLE_PREFER_STATIC_SDL}")
message(STATUS "BUBBLE_MSVC_STATIC_RUNTIME=${BUBBLE_MSVC_STATIC_RUNTIME}")
message(STATUS "VCPKG_TARGET_TRIPLET=${VCPKG_TARGET_TRIPLET}")
message(STATUS "BUBBLE_ENABLE_INSTALL=${BUBBLE_ENABLE_INSTALL}")
message(STATUS "BUBBLE_ENABLE_CPACK=${BUBBLE_ENABLE_CPACK}")
if(DEFINED BUBBLE_SDL2_TTF_TARGET)
    message(STATUS "SDL2_ttf target: ${BUBBLE_SDL2_TTF_TARGET}")
endif()
if(DEFINED BUBBLE_SDL2_MIXER_TARGET)
    message(STATUS "SDL2_mixer target: ${BUBBLE_SDL2_MIXER_TARGET}")
endif()

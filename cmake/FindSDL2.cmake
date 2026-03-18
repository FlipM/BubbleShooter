# FindSDL2.cmake — fallback finder used when SDL2's own config is absent.
# Sets SDL2_FOUND, SDL2_INCLUDE_DIRS, SDL2_LIBRARIES, SDL2_VERSION.
#
# Search hints (override via -DSDL2_ROOT=<path>):
#   Windows: %ProgramFiles%\SDL2, C:\SDL2
#   Linux:   /usr, /usr/local

include(FindPackageHandleStandardArgs)

# ── Try pkg-config first ─────────────────────────────────────────────────────
find_package(PkgConfig QUIET)
if(PKG_CONFIG_FOUND)
    pkg_check_modules(PC_SDL2 QUIET sdl2)
endif()

# ── Header ───────────────────────────────────────────────────────────────────
find_path(SDL2_INCLUDE_DIR
    NAMES SDL.h
    HINTS
        ${SDL2_ROOT}
        ${PC_SDL2_INCLUDE_DIRS}
        ENV SDL2DIR
    PATH_SUFFIXES SDL2 include/SDL2 include
)

# ── Library ──────────────────────────────────────────────────────────────────
find_library(SDL2_LIBRARY
    NAMES SDL2 SDL2-static
    HINTS
        ${SDL2_ROOT}
        ${PC_SDL2_LIBRARY_DIRS}
        ENV SDL2DIR
    PATH_SUFFIXES lib lib64
)

find_library(SDL2MAIN_LIBRARY
    NAMES SDL2main
    HINTS
        ${SDL2_ROOT}
        ${PC_SDL2_LIBRARY_DIRS}
        ENV SDL2DIR
    PATH_SUFFIXES lib lib64
)

# ── Version ──────────────────────────────────────────────────────────────────
if(SDL2_INCLUDE_DIR AND EXISTS "${SDL2_INCLUDE_DIR}/SDL_version.h")
    file(STRINGS "${SDL2_INCLUDE_DIR}/SDL_version.h" _ver
        REGEX "^#define SDL_(MAJOR|MINOR|PATCHLEVEL)_VERSION")
    string(REGEX REPLACE ".*MAJOR_VERSION ([0-9]+).*" "\\1" _maj "${_ver}")
    string(REGEX REPLACE ".*MINOR_VERSION ([0-9]+).*" "\\1" _min "${_ver}")
    string(REGEX REPLACE ".*PATCHLEVEL ([0-9]+).*"    "\\1" _pat "${_ver}")
    set(SDL2_VERSION "${_maj}.${_min}.${_pat}")
endif()

# ── Result ───────────────────────────────────────────────────────────────────
find_package_handle_standard_args(SDL2
    REQUIRED_VARS SDL2_LIBRARY SDL2_INCLUDE_DIR
    VERSION_VAR   SDL2_VERSION
)

if(SDL2_FOUND AND NOT TARGET SDL2::SDL2)
    set(SDL2_INCLUDE_DIRS ${SDL2_INCLUDE_DIR})
    set(SDL2_LIBRARIES    ${SDL2_LIBRARY})
    if(SDL2MAIN_LIBRARY)
        list(PREPEND SDL2_LIBRARIES ${SDL2MAIN_LIBRARY})
    endif()

    add_library(SDL2::SDL2 UNKNOWN IMPORTED)
    set_target_properties(SDL2::SDL2 PROPERTIES
        IMPORTED_LOCATION             "${SDL2_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${SDL2_INCLUDE_DIR}"
    )
endif()

mark_as_advanced(SDL2_INCLUDE_DIR SDL2_LIBRARY SDL2MAIN_LIBRARY)

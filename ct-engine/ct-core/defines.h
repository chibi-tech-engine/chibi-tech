#pragma once

// Platform detection
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#    define CT_PLATFORM_WINDOWS 1
#    define CT_PLATFORM_LINUX 0
#    define CT_PLATFORM_UNIX 0
#    ifndef _WIN64
#        error "64-bit is required on Windows!"
#    endif
#elif defined(__linux__) || defined(__gnu_linux__)
// Linux OS
#    define CT_PLATFORM_WINDOWS 0
#    define CT_PLATFORM_LINUX 1
#    define CT_PLATFORM_UNIX 0
#elif defined(__unix__)
// Catch anything not caught by the above.
#    define CT_PLATFORM_WINDOWS 0
#    define CT_PLATFORM_LINUX 0
#    define CT_PLATFORM_UNIX 1
#elif defined(_POSIX_VERSION)
// Posix
#    define CT_PLATFORM_WINDOWS 0
#    define CT_PLATFORM_LINUX 0
#    define CT_PLATFORM_POSIX 1
#else
#    error "Unknown platform!"
#endif

// Linux has 3 major windowing libraries:
// - Wayland
// - XLib
// - XCB ("modern" XLib)
#if CT_PLATFORM_LINUX
#    if _ENABLE_WAYLAND_
#        define CT_WAYLAND_WINDOW 1
#        define CT_XLIB_WINDOW 0
#    else
#        define CT_WAYLAND_WINDOW 0
#        define CT_XLIB_WINDOW 1
#    endif
#else // CT_PLATFORM_LINUX
#    define CT_WAYLAND_WINDOW 0
#    define CT_XLIB_WINDOW 0
#endif

#ifdef CT_EXPORT
// Exports
#    ifdef _MSC_VER
#        define CT_API __declspec(dllexport)
#    else
#        define CT_API __attribute__((visibility("default")))
#    endif
#else
// Imports
#    ifdef _MSC_VER
#        define CT_API __declspec(dllimport)
#    else
#        define CT_API
#    endif
#endif

// Compile mode detection
#if _DEBUG
#    define CT_DEBUG 1
#    define CT_RELEASE 0
#else
#    define CT_RELEASE 1
#    define CT_DEBUG 0
#endif

// Inlining
#if defined(__clang__) || defined(__gcc__)
#    define CT_INLINE __attribute__((always_inline)) inline
#    define CT_NOINLINE __attribute__((noinline))
#elif defined(_MSC_VER)
#    define CT_INLINE __forceinline
#    define CT_NOINLINE __declspec(noinline)
#else
#    define CT_INLINE static inline
#    define CT_NOINLINE
#endif

// Deprecation
#if defined(__clang__) || defined(__gcc__) || defined(__GNUC__)
#    define CT_DEPRECATED(message) __attribute__((deprecated(message)))
#elif defined(_MSC_VER)
#    define CT_DEPRECATED(message) __declspec(deprecated(message))
#else
#    error "Unsupported compiler - don't know how to define deprecations!"
#endif

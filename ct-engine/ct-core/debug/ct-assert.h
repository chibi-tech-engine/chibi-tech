#pragma once

#include "../defines.h"
#include "../types.h"

// Disable assertions by commenting out the below line.
// todo: meson configuration for this
#define CT_ASSERTIONS_ENABLED

// Always define debug_break in case it is ever needed outside assertions (i.e fatal log errors)
// Try via __has_builtin first.
#if defined(__has_builtin) && !defined(__ibmxl__)
#    if __has_builtin(__builtin_debugtrap)
#        define ct_debug_break() __builtin_debugtrap()
#    elif __has_builtin(__debugbreak)
#        define ct_debug_break() __debugbreak()
#    endif
#endif

// If not setup, try the old way.
#if !defined(ct_debug_break)
#    if defined(__clang__) || defined(__gcc__) || defined(__GNUC__)
/** @brief Causes a debug breakpoint to be hit. */
#        define ct_debug_break() __builtin_trap()
#    elif defined(_MSC_VER)
#        include <intrin.h>
/** @brief Causes a debug breakpoint to be hit. */
#        define ct_debug_break() __debugbreak()
#    else
// Fall back to x86/x86_64
#        define ct_debug_break() asm { int 3 }
#    endif
#endif

#ifdef CT_ASSERTIONS_ENABLED
/**
 * @brief Reports an assertion failure. Note that this is not the assertion itself,
 * just a reporting of an assertion failure that has already occurred.
 * @param expression The expression to be reported.
 * @param message A custom message to be reported, if provided.
 * @param file The path and name of the file containing the expression.
 * @param line The line number in the file where the assertion failure occurred.
 */
CT_API void report_assertion_failure(const char* expression, const char* message, const char* file, s32 line);

/**
 * @brief Asserts the provided expression to be true, and logs a failure if not.
 * Also triggers a breakpoint if debugging.
 * @param expr The expression to be evaluated.
 */
#    define CT_ASSERT(expr)                                                                                                      \
        {                                                                                                                        \
            if (expr) {                                                                                                          \
            } else {                                                                                                             \
                report_assertion_failure(#expr, "", __FILE__, __LINE__);                                                         \
                ct_debug_break();                                                                                                \
            }                                                                                                                    \
        }

/**
 * @brief Asserts the provided expression to be true, and logs a failure if not.
 * Allows the user to specify a message to accompany the failure.
 * Also triggers a breakpoint if debugging.
 * @param expr The expression to be evaluated.
 * @param message The message to be reported along with the assertion failure.
 */
#    define CT_ASSERT_MSG(expr, message)                                                                                         \
        {                                                                                                                        \
            if (expr) {                                                                                                          \
            } else {                                                                                                             \
                report_assertion_failure(#expr, message, __FILE__, __LINE__);                                                    \
                ct_debug_break();                                                                                                \
            }                                                                                                                    \
        }

#    if CT_DEBUG
/**
 * @brief Asserts the provided expression to be true, and logs a failure if not.
 * Also triggers a breakpoint if debugging.
 * NOTE: Only included in debug builds; otherwise this is preprocessed out.
 * @param expr The expression to be evaluated.
 */
#        define CT_ASSERT_DEBUG(expr)                                                                                            \
            {                                                                                                                    \
                if (expr) {                                                                                                      \
                } else {                                                                                                         \
                    report_assertion_failure(#expr, "", __FILE__, __LINE__);                                                     \
                    ct_debug_break();                                                                                            \
                }                                                                                                                \
            }
#    else
#        define CT_ASSERT_DEBUG(expr) // Does nothing at all
#    endif

#else
#    define CT_ASSERT(expr)              // Does nothing at all
#    define CT_ASSERT_MSG(expr, message) // Does nothing at all
#    define CT_ASSERT_DEBUG(expr)        // Does nothing at all
#endif

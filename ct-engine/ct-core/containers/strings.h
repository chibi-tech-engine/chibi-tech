#pragma once

#include "defines.h"
#include <stddef.h>

CT_API int ct_strlen(const char* str);
CT_API bool ct_strcmp(const char* str_a, const char* str_b);
CT_API int ct_strformat(char* s, size_t n, const char* format, ...);
CT_API void ct_write_to_console(const char* format, ...);

#include "strings.h"

CT_API int
ct_strlen(const char* str)
{
	int result = 0;
	for (const char* iter = str; *iter; iter++) result += 1;
	return result;
}

CT_API bool
ct_strcmp(const char* str_a, const char* str_b)
{
    const int str_a_len = ct_strlen(str_a);
    const int str_b_len = ct_strlen(str_b);

    if (str_a_len != str_b_len) return false;

    for (int i = 0; i < str_a_len; ++i)
    {
        if (str_a[i] != str_b[i]) return false;
    }

    return true;
}

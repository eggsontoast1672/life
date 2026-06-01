#include "str_utils.h"

#include <stddef.h>
#include <string.h>

bool str_strip_prefix(const char **str, const char *prefix)
{
    const size_t str_length = strlen(*str);
    const size_t prefix_length = strlen(prefix);

    if (str_length < prefix_length)
    {
        return false;
    }

    for (size_t i = 0; i < prefix_length; i++)
    {
        if ((*str)[i] != prefix[i])
        {
            return false;
        }
    }

    *str += prefix_length;
    return true;
}

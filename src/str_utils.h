#ifndef LIFE_STR_UTILS_H
#define LIFE_STR_UTILS_H

#include <stdbool.h>

/// Strip a prefix from a string.
///
/// This function attempts to remove `prefix` from the beginning of `str`. If `str` begins with
/// `prefix`, the pointer is advanced past the portion of the string constituting the prefix and
/// written back to `str`. Otherwise, `str` remains unchanged and `false` is returned to signal
/// failure.
///
/// @param str The string to be stripped.
/// @param prefix The prefix to strip.
/// @return `true` if the prefix was stripped, `false` otherwise.
bool str_strip_prefix(const char **str, const char *prefix);

#endif

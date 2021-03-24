/* common_str:
 *
 * Commonly used string related functions.
 */

#ifndef COMMONSTR_H
#include "common_str.h"
#endif

/* is_empty_line: returns true if the specified string represents an
 * empty line.
 *
 * Parameters (in order):
 *
 * # string variable/literal to check.
 *
 * Note: by definition, if string variable parameter is NULL, it is
 * not an empty line and return value is false.
 *
 * Returns: bool value to indicate whether specified string represents
 * an empty line or not; true = yes, false = no.
 */
bool is_empty_line(const char *line)
{
    return (line == NULL) ? false : ((*line) == '\n');
}

/* str_has_char: returns true if the specified string contains the
 * specified character.
 *
 * Parameters (in order):
 *
 * # string variable/literal to check for character.
 * # character to check for in string.
 *
 * Note: by definition, if string variable/literal parameter is NULL,
 * it does not conain specified character and return value is false.
 *
 * Returns: bool value to indicate wheteher specified string contains
 * specified character; true = yes, false = no.
 */
bool str_has_char(const char *str, const char c)
{
    return (str == NULL) ? false : (strchr(str, c) != NULL);
}

/* char_pos_str: returns position of specified character in specified
 * string if found; otherwise, returns -1.
 *
 * Paramerters (in order):
 *
 * # string variable/literal to check for character.
 * # character to check for in string.
 *
 * Note: passing NULL for string variable/literal parameter results in
 * undefined behavior.
 *
 * Returns: position of specified character in string if found, -1
 * otherwise.
 */
long char_pos_str(const char *str, const char c)
{
    char *found = strchr(str, c);
    return (found != NULL) ? (found - str) : -1;
}

/* EOF. */

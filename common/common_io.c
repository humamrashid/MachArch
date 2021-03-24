/* common_io:
 *
 * Commonly used functions for I/O.
 */

#ifndef _STDLIB_H
#include <stdlib.h>
#endif

#ifndef _STDIO_H
#include <stdio.h>
#endif

#ifndef COMMONERR_H
#include "common_err.h"
#endif

#ifndef COMMONIO_H
#include "common_io.h"
#endif

/* file_read_error: print file reading error to specified file stream
 * with specified file name and exit if error code parameter is
 * EXIT_FAILURE.
 *
 * Parameters (in order):
 *
 * # file stream to print error message to.
 * # string for file name.
 * # integer for error code.
 *
 * Note: passing NULL for file stream or file name results in no
 * operation being done.
 *
 * Note: this function only terminates calling process if error code
 * parameter is EXIT_FAILURE (defined in stdlib.h), in which case the
 * same error code is passed to the operating system.
 *
 * Returns: N/A.
 */
void file_read_error(
        FILE *stream,
        const char *file_name,
        const int error_code)
{
    if ((stream == NULL) || (file_name == NULL)) return;
    fprintf(stream, "ERROR: unable to read file %s!\n", file_name);
    exit_on_exit_fail(error_code, error_code);
    return;
}

/* file_write_error: print file writing error to specified file stream
 * with specified file name and exit if error code parameter is
 * EXIT_FAILURE.
 *
 * Parameters (in order):
 *
 * # file stream to print error message to.
 * # string for file name.
 * # integer for error code.
 *
 * Note: passing NULL for file stream or file name results in no
 * operation being done.
 *
 * Note: this function only terminates calling process if error code
 * parameter is EXIT_FAILURE (defined in stdlib.h), in which case the
 * same error code is passed to the operating system.
 *
 * Returns: N/A.
 */
void file_write_error(
        FILE *stream,
        const char *file_name,
        const int error_code)
{
    if ((stream == NULL) || (file_name == NULL)) return;
    fprintf(stream, "ERROR: unable to write to file %s!\n",
            file_name);
    exit_on_exit_fail(error_code, error_code);
    return;
}

/* EOF. */

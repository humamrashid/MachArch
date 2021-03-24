/* common_err:
 *
 * Commonly used functions related to general error conditions.
 */

#ifndef COMMONERR_H
#define COMMONERR_H

#ifndef _STDLIB_H
#include <stdlib.h>
#endif

#ifndef _STDIO_H
#include <stdio.h>
#endif

/* exit_on_exit_fail: exit if error code parameter is EXIT_FAILURE.
 *
 * Parameters (in order):
 *
 * # integer for error code.
 * # integer for error code.
 *
 * Note: this function only terminates calling process if first error
 * code parameter is EXIT_FAILURE (defined in stdlib.h), in which case
 * the second error code parameter is passed to the operating system.
 *
 * Returns: N/A.
 */
void exit_on_exit_fail(
        const int,
        const int);

/* memory_alloc_error: print memory allocation error message to
 * specified file stream and exit if error code is EXIT_FAILURE.
 *
 * Parameters (in order):
 *
 * # file stream to print error message to.
 * # integer for error code.
 *
 * Note: passing NULL for file stream results in no operation being
 * done.
 *
 * Note: this function only terminates calling process if error code
 * parameter is EXIT_FAILURE (defined in stdlib.h), in which case the
 * same error code is passed to the operating system.
 *
 * Returns: N/A.
 */
void memory_alloc_error(
        FILE*,
        const int);

/* memory_bounds_error: print memory access violation error message to
 * specified file stream and exit if error code is EXIT_FAILURE.
 *
 * Parameters (in order):
 *
 * # file stream to print error message to.
 * # position for which memory access is in violation.
 * # integer for error code.
 *
 * Note: passing NULL for file stream results in no operation being
 * done.
 *
 * Note: this function only terminates calling process if error code
 * parameter is EXIT_FAILURE (defined in stdlib.h), in which case the
 * same error code is passed to the operating system.
 *
 * Returns: N/A.
 */
void memory_bounds_error(
        FILE*,
        const int,
        const int);

#endif

/* EOF. */

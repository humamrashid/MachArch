/* memory.c:
 *
 * A simple, extensible memory model.
 */

#ifndef _STDLIB_H
#include <stdlib.h>
#endif

#ifndef MEMORY_H
#include "memory.h"
#endif

#define MEM_SKIP_VAL 0x00

/* m_allocate: allocate memory of specified size.
 *
 * Parameters (in order):
 *
 * # pointer to memory variable.
 * # amount of memory to allocate (i.e., number of memory units).
 *
 * Note: initial value of memory units is undefined. Use
 * m_allocate_init() if setting initial values is prioritized.
 *
 * Returns: bool value to indicate status of allocation; true =
 * success, false = failure.
 */
bool m_allocate(
        memory *m,
        const unsigned int size)
{
    return ((*m = malloc(size * sizeof(int))) != NULL);
}

/* m_allocate_init: allocate memory of specified size and having
 * specified inital values.
 *
 * Note: setting values for all memory units is a linear time
 * operation; use m_allocate() if efficiency is prioritized.
 *
 * Parameters (in order):
 *
 * # pointer to memory variable.
 * # amount of memory to allocate (i.e., number of memory units).
 * # initial value for all memory units allocated.
 *
 * Returns: bool value to indicate status of allocation; true =
 * success, false = failure.
 */
bool m_allocate_init(
        memory *m,
        const unsigned int size,
        const int init_val)
{
    *m = malloc(size * sizeof(int));
    if (*m == NULL) return false;
    m_set_values(m, 0, size, init_val);
    return true;
}

/* m_deallocate: deallocate memory previously allocated.
 *
 * Parameters (in order):
 *
 * # pointer to memory variable.
 *
 * Note: memory must have been allocated beforehand. Passing pointers
 * to unallocated memory results in undefined behavior. Passing NULL
 * results in no operation being performed.
 *
 * Returns: N/A.
 */
void m_deallocate(memory *m)
{
    if (m == NULL) return;
    free(*m);
    return;
}

/* m_get_value: return value contained in memory at specified
 * position.
 *
 * Parameters (in order):
 *
 * # memory variable.
 * # unsigned value for position of memory unit.
 *
 * Note: memory must have been allocated beforehand. Passing NULL or
 * variables containing unallocated memory results in undefined
 * behavior.
 *
 * Note: position parameter must be within bounds of memory. Passing
 * values beyond bounds results in undefined behavior.
 *
 * Returns: value contained in memory at specified position.
 */
int m_get_value(
        const memory m,
        const unsigned int pos)
{
    return *(m + pos);
}

/* m_set_value: set value contained in memory at specified position.
 *
 * Parameters (in order):
 *
 * # pointer to memory variable.
 * # unsigned value for position of memory unit.
 * # value to set at specified position in memory.
 *
 * Note: memory must have been allocated beforehand. Passing pointers
 * to unallocated memory results in undefined behavior. Passing NULL
 * results in no operation being done.
 *
 * Note: position parameter must be within bounds of memory. Passing
 * values beyond bounds results in undefined behavior.
 *
 * Returns: N/A.
 */
void m_set_value(
        memory *m,
        const unsigned int pos,
        const int value)
{
    if (m == NULL) return;
    *((*m) + pos) = value;
    return;
}

/* m_set_values: set one value for all memory units in specified
 * range.
 * 
 * Note: setting values for memory units in a range is a linear time
 * operation.
 *
 * Parameters (in order):
 *
 * # pointer to memory variable.
 * # unsigned value indicating starting position of range.
 * # unsigned value indicating ending position of range.
 * # value to set for all memory units in range.
 *
 * Note: memory must have been allocated beforehand. Passing pointers
 * to unallocated memory results in undefined behavior. Passing NULL
 * results in no operation being done.
 *
 * Note: range must be within bounds of memory with starting position
 * being less than or equal to ending position. Passing values not
 * meeting these requirements results in undefined behavior.
 *
 * Returns: N/A.
 */
void m_set_values(
        memory *m,
        const unsigned int start,
        const unsigned int end,
        const int value)
{
    if (m == NULL) return;
    unsigned int i;
    for (i = start; i < end; i++) *((*m) + i) = value;
    return;
}

/* m_clear: clear memory in specified range (i.e., set all values
 * contained in memory units within range to 0).
 *
 * # Note: clearing memory in a range is a linear time operation.
 *
 * Parameters (in order):
 *
 * # pointer to memory variable.
 * # unsigned value indicating starting position of range.
 * # unsigned value indicaing ending posiion of range.
 *
 * Note: memory must have been allocated beforehand. Passing pointers
 * to unallocated memory results in undefined behavior. Passing NULL
 * results in no operation being done.
 *
 * Note: range must be within bounds of memory with starting position
 * being less than or equal to ending position. Passing values not
 * meeting these requirements results in undefined behavior.
 *
 * Returns: N/A.
 */
void m_clear(
        memory *m,
        const unsigned int start,
        const unsigned int end)
{
    if (m == NULL) return;
    m_set_values(m, start, end, 0);
    return;
}

/* m_extend: extend memory size by specified amount.
 *
 * Parameters (in order):
 *
 * # pointer to memory variable.
 * # unsigned value for original memory size.
 * # unsigned value for extension size.
 *
 * Note: memory must have been allocated beforehand. Passing pointers
 * to unallocated memory results in undefined behavior.
 *
 * Note: by definition, if pointer to memory variable parameter is
 * NULL, memory extension fails and return value is false.
 *
 * Note: extension size is the amount of memory to add to original
 * amount. Therefore, the new memory size is (original size +
 * extension size).
 *
 * Note: values contained in memory units added to original amount are
 * undefined. Use m_extend_init() if setting initial values is
 * prioritized.
 *
 * Returns: bool value indicating status of extension; true = success,
 * false = failure.
 */
bool m_extend(
        memory *m,
        const unsigned int size,
        const unsigned int e_size)
{
    if (m == NULL) return false;
    memory temp;
    if (!m_allocate(&temp, size + e_size)) return false;
    m_copy_mem(*m, 0, size, &temp, 0, size);
    *m = temp;
    return true;
}

/* m_extend_init: extend memory size by specified amount having
 * specified initial values.
 *
 * Note: setting values for added memory units is a linear time
 * operation; use m_extend() if efficiency is prioritized.
 *
 * Parameters (in order):
 *
 * # pointer to memory variable.
 * # unsigned value for original memory size.
 * # unsigned value for extension size.
 * # initial value for all memory units added.
 *
 * Note: memory must have been allocated beforehand. Passing pointers
 * to unallocated memory results in undefined behavior.
 *
 * Note: by definition, if pointer to memory variable parameter is
 * NULL, memory extension fails and return value is false.
 *
 * Note: extension size is the amount of memory to add to original
 * amount. Therefore, the new memory size is (original size +
 * extension size).
 *
 * Returns: bool value indicating status of extension; true = success,
 * false = failure.
 */
bool m_extend_init(
        memory *m,
        const unsigned int size,
        const unsigned int e_size,
        const int init_val)
{
    if ((m == NULL) || (!m_extend(m, size, e_size))) return false;
    m_set_values(m, size, size + e_size, init_val);
    return true;
}

/* m_copy_arr: copy values (in order) contained within specified range
 * of an integer array into specified range of memory.
 *
 * Note: sequential copying is a linear time operation.
 *
 * Parameters (in order):
 *
 * # integer array to copy from.
 * # unsigned value indicating starting position of range for integer
 * array.
 * # unsigned value indicaing ending posiion of range for integer
 * array.
 * # pointer to memory variable to copy to.
 * # unsigned value indicating starting position of range for memory.
 * # unsigned value indicating ending position of range for memory.
 *
 * Note: memory must have been allocated beforehand. Passing pointers
 * to unallocated memory results in undefined behavior. Passing NULL
 * results in no operation being done.
 *
 * Note: ranges must be within bounds with starting position being
 * less than or equal to ending position. Passing values not meeting
 * these requirements results in undefined behavior.
 *
 * Note: no assumptions are made regarding relative range sizes of
 * either the integer array or memory; copying is carried out until
 * either ending position is reached. If integer array range size is
 * larger, copying is cut off at this point; if memory range size is
 * larger, copying is finished with remaining memory within its range
 * left untouched.
 *
 * Returns: N/A.
 */
void m_copy_arr(
        const int arr[],
        const unsigned int start1,
        const unsigned int end1,
        memory *m,
        const unsigned int start2,
        const unsigned int end2)
{
    if (arr == NULL || m == NULL) return;
    unsigned int i, j;
    for (i = start2, j = start1; ((i < end2) && (j < end1)); i++, j++)
        *((*m) + i) = *(arr + j);
    return;
}

/* m_copy_mem: copy values (in order) contained within specified range
 * of one memory into specified range of another.
 *
 * Note: sequential copying is a linear time operation.
 *
 * Parameters (in order):
 *
 * # memory variable to copy from (source).
 * # unsigned value indicating starting position of range for memory
 * to copy from.
 * # unsigned value indicaing ending posiion of range for memory to
 * copy from.
 * # pointer to memory variable to copy to (destination).
 * # unsigned value indicating starting position of range for memory
 * to copy to.
 * # unsigned value indicating ending position of range for memory to
 * copy to.
 *
 * Note: memory must have been allocated beforehand. Passing pointers
 * to unallocated memory results in undefined behavior. Passing NULL
 * results in no operation being done.
 *
 * Note: ranges must be within bounds with starting position being
 * less than or equal to ending position. Passing values not meeting
 * these requirements results in undefined behavior.
 *
 * Note: no assumptions are made regarding relative range sizes of
 * either source or destination; copying is carried out until either
 * ending position is reached. If source range size is larger, copying
 * is cut off at this point; if destination range size is larger,
 * copying is finished with remaining memory within its range left
 * untouched.
 *
 * Returns: N/A.
 */
void m_copy_mem(
        const memory src,
        const unsigned int start1,
        const unsigned int end1,
        memory *dest,
        const unsigned int start2,
        const unsigned int end2)
{
    m_copy_arr(src, start1, end1, dest, start2, end2);
    return;
}

/* m_get_copy: copy a specified range of memory.
 *
 * Note: sequential copying is a linear time operation.
 *
 * Parameters (in order):
 *
 * # memory variable to copy from (source).
 * # pointer to memory variable to copy to (destination).
 * # unsigned value indicating starting position of range.
 * # unsigned value indicating ending position of range.
 *
 * Note: source memory must have been allocated beforehand. Passing
 * variables containing unallocated memory for source results in
 * undefined behavior. Destination memory must be NULL. Passing NULL
 * for source memory or non-NULL for destination memory results in
 * false return value.
 *
 * Note: range must be within bounds of source memory with starting
 * position being less than or equal to ending position. Passing
 * values not meeting these requirements results in undefined
 * behavior.
 *
 * Returns: bool value to indicate status of copying; true = success,
 * false = failure.
 */
bool m_get_copy(
        const memory m1,
        memory *m2,
        const unsigned int start,
        const unsigned int end)
{
    if ((m1 == NULL) || ((*m2) != NULL)) return false;
    if (m_allocate(m2, end - start))
    {
        m_copy_arr(m1, start, end, m2, 0, end);
        return true;
    }
    return false;
}

/* m_eq_check: check if specified ranges of two memories are equal
 * (i.e., contain the same exact values in the same exact order).
 *
 * Note: sequential checking for equality is a linear time operation.
 *
 * Parameters (in order):
 *
 * # memory variable.
 * # unsigned value indicating starting position of range for first
 * memory parameter.
 * # unsigned value indicating ending position of range for first
 * memory parameter.
 * # memory variable.
 * # unsigned value indicating starting position of range for second
 * memory parameter.
 * # unsigned value indicating ending position of range for second
 * memory parameter.
 *
 * Note: memory must have been allocated beforehand. Passing variables
 * containing unallocated memory results in undefined behavior.
 *
 * Note: by definition, if both memory variable parameters are NULL,
 * they are considered equal; if only one is NULL, they are considered
 * unequal.
 *
 * Note: ranges must be within bounds with starting position being
 * less than or equal to ending position. Passing values not meeting
 * these requirements results in undefined behavior.
 *
 * Note: no assumptions are made regarding relative range sizes of
 * either source or destination. By definition, ranges not equal in
 * size are not equal. Checking for equality is carried out until
 * either ending position is reached.
 *
 * Returns: bool value indicating equality; true = equal, false = not
 * equal.
 */
bool m_eq_check(
        const memory m1,
        const unsigned int start1,
        const unsigned int end1,
        const memory m2,
        const unsigned int start2,
        const unsigned int end2)
{
    if ((m1 == NULL) && (m2 == NULL)) return true;
    else if ((m1 == NULL) && (m2 != NULL)) return false;
    else if ((m1 != NULL) && (m2 == NULL)) return false;
    else if ((end1 - start1) != (end2 - start2)) return false;
    unsigned int i, j;
    for (i = start2, j = start1; ((i < end2) && (j < end1)); i++, j++)
        if (*(m1 + i) != *(m2 + j)) return false;
    return true;
}

/* m_search: return first position of key in specified unsorted range
 * of memory if found; otherwise, return -1.
 *
 * Note: specified range of memory is assumed to be unsorted and
 * searching is therefore a linear time operation. If memory can be
 * assumed to be sorted, use m_search_s() instead to search in
 * logarithmic time.
 *
 * Parameters (in order):
 *
 * # memory variable.
 * # unsigned value indicating starting position of range.
 * # unsigned value indicating ending position of range.
 * # key to search for in specified range.
 *
 * Note: memory must have been allocated beforehand. Passing NULL or
 * variables containing unallocated memory results in undefined
 * behavior.
 *
 * Note: range must be within bounds of memory with starting position
 * being less than or equal to ending position. Passing values not
 * meeting these requirements results in undefined behavior.
 *
 * Returns: first position of key if found within specified range of
 * memory, -1 otherwise.
 */
int m_search(
        const memory m,
        const unsigned int start,
        const unsigned int end,
        int key)
{
    unsigned int i;
    for (i = start; i < end; i++) if (*(m + i) == key) return i;
    return -1;
}

/* m_search_s: return first position of key in specified sorted range
 * of memory if found; otherwise, return -1.
 *
 * Note: specified range of memory is assumed to be sorted and
 * searching is done in logarithmic time. If memory cannot be assumed
 * to be sorted, use m_search() instead.
 *
 * Parameters (in order):
 *
 * # memory variable.
 * # value indicating starting position of range.
 * # value indicating ending position of range.
 * # key to search for in specified range.
 *
 * Note: memory must have been allocated beforehand. Passing NULL or
 * variables containing unallocated memory results in undefined
 * behavior.
 *
 * Note: range must be within bounds of memory with starting position
 * being less than or equal to ending position. Passing values not
 * meeting these requirements results in undefined behavior.
 *
 * Returns: first position of key if found within specified range of
 * memory, -1 otherwise.
 */
int m_search_s(
        const memory m,
        int low,
        int high,
        int key)
{
    if (low > high) return -1;
    unsigned int mid = (low + high) / 2;
    if (*(m + mid) == key)
        return mid;
    else if (*(m + mid) < key)
        return m_search_s(m, mid + 1, high, key);
    else
        return m_search_s(m, low, mid - 1, key);
}

/* m_print_memory: print out memory addresses and values in specified
 * range to specified file stream.
 *
 * Note: presentation is up to implementer to decide. This
 * implementation presents all memory addresses and their values line
 * by line but skips over MEM_SKIP_VAL (defined in this file) values
 * and their addresses if and when more than two of such appear
 * consecutively. Addresses and their values are printed in 8-digit,
 * zero-filled, base 16 numbers.
 *
 * Parameters (in order):
 *
 * # memory variable.
 * # unsigned value indicating starting position of range.
 * # unsigned value indicating ending position of range.
 * # file stream to print out to.
 *
 * Note: memory must have been allocated beforehand. Passing variables
 * containing unallocated memory results in undefined behavior.
 *
 * Note: passing NULL for memory variable or file stream results in no
 * operation being done.
 *
 * Note: range must be within bounds of memory with starting position
 * being less than or equal to ending position. Passing values not
 * meeting these requirements results in undefined behavior.
 *
 * Returns: N/A.
 */
void m_print_memory(
        const memory m,
        const unsigned int start,
        const unsigned int end,
        FILE *stream)
{
    if ((m == NULL) || (stream == NULL)) return;
    unsigned int i = start,
                 skip_lines = 0;
    while (i < end)
    {
        if (*(m + i) == MEM_SKIP_VAL)
        {
            skip_lines++;
            if ((skip_lines > 1) && (i < (end - 1)))
            {
                i++;
                if (skip_lines == 2) fprintf(stream, ". . . . .\n");
                continue;
            }
        } else skip_lines = 0;
        fprintf(stream, "0x%08X:\t0x%08X\n", i, *(m + i));
        i++;
    }
    return;
}

/* EOF. */

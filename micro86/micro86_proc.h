/* micro86_proc.h:
 *
 * A simple 4-register processor model for the micro86 emulator.
 *
 * Note: initialization functions should be used before using other
 * functions declared here; register values are undefined by default.
 */

#ifndef _STDIO_H
#include <stdio.h>
#endif

#ifndef _STDBOOL_H
#include <stdbool.h>
#endif

#ifndef MICRO86PROC_H
#define MICRO86PROC_H

#define ZERO_BIT_TRUE   0x01
#define ZERO_BIT_FALSE  0x00
#define SIGN_BIT_TRUE   0x01
#define SIGN_BIT_FALSE  0x00

/* Type: micro86_proc.
 *
 * A simple processor model consisting of the following:
 *
 * # acc: accumulator register.
 *
 * The accumulator contains the result of the latest operation.
 *
 * # ir: instruction register.
 *
 * The instruction register points to the current instruction being
 * executed.
 *
 * # ip: instruction pointer register.
 *
 * The instruction pointer points to the next instruction to be
 * executed.
 *
 * # flags: flags register.
 *
 * The flags register is used for comparison operations. It consists
 * of a zero bit (rightmost) and a sign bit (second to rightmost).
 *
 * # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
 * WARNING: micro86_proc registers should not be accessed or modified
 * directly. The functions declared below are to be used for such
 * purposes.
 * # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
 */
typedef struct
{
    int acc;
    unsigned int ir,
                 ip,
                 flags;
} micro86_proc;

/* m86_proc_init: initialize micro86_proc registers with default value
 * zero.
 *
 * Note: most other functions declared here require micro86_proc
 * register to be initialized.
 *
 * Parameters (in order):
 *
 * # pointer to micro86_proc variable.
 *
 * Note: passing NULL for micro86_proc variable parameter results in
 * no operation being performed.
 *
 * Returns: N/A.
 */
void m86_proc_init(micro86_proc*);

/* m86_proc_init_values: initialize micro86_proc registers with
 * specified values.
 *
 * Note: most other functions declared here require micro86_proc
 * register to be initialized.
 *
 * Parameters (in order):
 * 
 * # pointer to micro86_proc variable.
 * # value for accumulator register.
 * # unsigned value for instruction register.
 * # unsigned value for instruction pointer register.
 *
 * Note: passing NULL for micro86_proc variable parameter results in
 * no operation being performed.
 *
 * Note: the flags register should only be handled using the functions
 * for setting the zero and sign bits declared below.
 *
 * Returns: N/A.
 */
void m86_proc_init_values(
        micro86_proc*,
        const int,
        const unsigned int,
        const unsigned int);

/* m86_get_acc_reg: return value contained in accumulator.
 *
 * Parameters (in order):
 *
 * # micro86_proc variable.
 *
 * Returns: value contained in accumulator register.
 */
int m86_get_acc_reg(const micro86_proc);

/* m86_set_acc_reg: set value contained in accumulator.
 *
 * Parameters (in order):
 *
 * # pointer to micro86_proc variable.
 * # value for accumulator register.
 *
 * Note: passing NULL for micro86_proc variable parameter results in
 * no operation being performed.
 *
 * Returns: N/A.
 */
void m86_set_acc_reg(
        micro86_proc*,
        const int);

/* m86_get_ir_reg: return value contained in instruction register.
 *
 * Parameters (in order):
 *
 * # micro86_proc variable.
 *
 * Returns: unsigned value contained in instruction register.
 */
unsigned int m86_get_ir_reg(const micro86_proc);

/* m86_set_ir_reg: set value contained in instruction register.
 *
 * Parameters (in order):
 *
 * # pointer to micro86_proc variable.
 * # unsigned value for instruction register.
 *
 * Note: passing NULL for micro86_proc variable parameter results in
 * no operation being performed.
 *
 * Returns: N/A.
 */
void m86_set_ir_reg(
        micro86_proc*,
        const unsigned int);

/* m86_get_ip_reg: return value contained in instruction pointer.
 *
 * Parameters (in order):
 *
 * # micro86_proc variable.
 *
 * Returns: unsigned value contained in instruction pointer register.
 */
unsigned int m86_get_ip_reg(const micro86_proc);

/* m86_set_ip_reg: set value contained in instruction pointer.
 *
 * Parameters (in order):
 *
 * # pointer to micro86_proc variable.
 * # unisigned value for instruction pointer register.
 *
 * Note: passing NULL for micro86_proc variable parameter results in
 * no operation being performed.
 *
 * Returns: N/A.
 */
void m86_set_ip_reg(
        micro86_proc*,
        const unsigned int);

/* m86_get_flags_reg: return value contained in flags register.
 *
 * Note:
 *
 * # The zero bit is defined to be the rightmost bit.
 * # The sign bit is defined to be the second to rightmost bit.
 *
 * Parameters (in order):
 *
 * # micro86_proc variable.
 * 
 * Returns: unsigned value contained in flags register.
 *
 * Note: the zero bit is either ZERO_BIT_TRUE or ZERO_BIT_FALSE (both
 * defined in micro86_proc.h).
 *
 * Note: the sign bit is either SIGN_BIT_TRUE or SIGN_BIT_FALSE (both
 * defined in micro86_proc.h).
 */
unsigned int m86_get_flags_reg(const micro86_proc);

/* m86_get_flags_zb: return zero bit of value contained in flags
 * register.
 * 
 * Parameters (in order):
 *
 * # micro86_proc variable.
 *
 * Returns: unsigned zero bit of value contained in flags register.
 *
 * Note: the zero bit is either ZERO_BIT_TRUE or ZERO_BIT_FALSE (both
 * defined in micro86_proc.h).
 */
unsigned int m86_get_flags_zb(const micro86_proc);

/* m86_set_flags_zb: set zero bit of value contained in flags
 * register.
 *
 * Parameters (in order):
 *
 * # pointer to micro86_proc variable.
 * # bool value to indicate zero bit value.
 *
 * Note: passing NULL for micro86_proc variable parameter results in
 * no operation being performed.
 *
 * Note: if bool parameter is true, zero bit is set to ZERO_BIT_TRUE;
 * if it is false, zero bit is set to ZERO_BIT_FALSE (both defined in
 * micro86_proc.h).
 *
 * Returns: N/A.
 */
void m86_set_flags_zb(
        micro86_proc*,
        bool);

/* m86_get_flags_sb: return sign bit of value contained in flags
 * register.
 *
 * Parameters (in order):
 *
 * # micro86_proc variable.
 *
 * Returns: unsigned sign bit value contained in flags register.
 *
 * Note: the sign bit is either SIGN_BIT_TRUE or SIGN_BIT_FALSE (both
 * defined in micro86_proc.h).
 */
unsigned int m86_get_flags_sb(const micro86_proc);

/* m86_set_flags_sb: set sign bit of value contained in flags
 * register.
 *
 * Parameters (in order):
 *
 * # pointer to micro86_proc variable.
 * # bool value to indicate sign bit value.
 *
 * Note: passing NULL for micro86_proc variable parameter results in
 * no operation being performed.
 *
 * Note: if bool parameter is true, sign bit is set to SIGN_BIT_TRUE;
 * if it is false, sign bit is set to SIGN_BIT_FALSE (both defined in
 * micro86_proc.h).
 *
 * Returns: N/A.
 */
void m86_set_flags_sb(
        micro86_proc*,
        bool);

/* m86_print_proc: print processor (i.e., registers and their values)
 * to specified file stream.
 *
 * Note: presentation is up to implementer to decide. This
 * implementation simply prints out the registers and their values
 * in one line with 8-digit, zero-filled, base 16 numbers.
 *
 * Parameters (in order):
 *
 * # micro86_proc variable.
 * # file stream to print out to.
 *
 * Note: passing NULL for file stream results in no operation being
 * done.
 *
 * Returns: N/A.
 */
void m86_print_proc(
        const micro86_proc,
        FILE*);

#endif

/* EOF. */

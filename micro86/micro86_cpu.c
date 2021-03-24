/* micro86_cpu.c:
 *
 * A simple 4-register cpu model for the micro86 emulator.
 *
 * Note: initialization functions should be used before using other
 * functions declared here; register values are undefined by default.
 */

#ifndef MICRO86PROC_H
#include "micro86_proc.h"
#endif

#define DEF_REG_VAL 0x00

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
void m86_proc_init(micro86_proc *p)
{
    if (p == NULL) return;
    p->acc =
        p->ir =
        p->ip =
        p->flags = DEF_REG_VAL;
    return;
}

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
        micro86_proc *p,
        const int acc,
        const unsigned int ir,
        const unsigned int ip)
{
    if (p == NULL) return;
    p->acc = acc;
    p->ir = ir;
    p->ip = ip;
    return;
}

/* m86_get_acc_reg: return value contained in accumulator.
 *
 * Parameters (in order):
 *
 * # micro86_proc variable.
 *
 * Returns: value contained in accumulator register.
 */
int m86_get_acc_reg(const micro86_proc p)
{
    return p.acc;
}

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
        micro86_proc *p,
        const int acc)
{
    if (p == NULL) return;
    p->acc = acc;
    return;
}

/* m86_get_ir_reg: return value contained in instruction register.
 *
 * Parameters (in order):
 *
 * # micro86_proc variable.
 *
 * Returns: unsigned value contained in instruction register.
 */
unsigned int m86_get_ir_reg(const micro86_proc p)
{
    return p.ir;
}

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
        micro86_proc *p,
        const unsigned int ir)
{
    if (p == NULL) return;
    p->ir = ir;
    return;
}

/* m86_get_ip_reg: return value contained in instruction pointer.
 *
 * Parameters (in order):
 *
 * # micro86_proc variable.
 *
 * Returns: unsigned value contained in instruction pointer register.
 */
unsigned int m86_get_ip_reg(const micro86_proc p)
{
    return p.ip;
}

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
        micro86_proc *p,
        const unsigned int ip)
{
    if (p == NULL) return;
    p->ip = ip;
    return;
}

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
unsigned int m86_get_flags_reg(const micro86_proc p)
{
    return p.flags;
}

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
unsigned int m86_get_flags_zb(const micro86_proc p)
{
    return (p.flags & 0x01);
}

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
        micro86_proc *p,
        bool yes)
{
    if (p == NULL) return;
    p->flags = yes ? (p->flags | 0x01) : (p->flags & 0x02);
    return;
}

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
unsigned int m86_get_flags_sb(const micro86_proc p)
{
    return (p.flags >> 1);
}

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
        micro86_proc *p,
        bool yes)
{
    if (p == NULL) return;
    p->flags = yes ? (p->flags | 0x02) : (p->flags & 0x01);
    return;
}

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
        const micro86_proc p,
        FILE *stream)
{
    if (stream == NULL) return;
    fprintf(stream, "Registers:"
            " acc: 0x%08X"
            " ip: 0x%08X"
            " flags: 0x%08X"
            " (ir: 0x%08X)\n",
            p.acc, p.ip, p.flags, p.ir);
    return;
}

/* EOF. */

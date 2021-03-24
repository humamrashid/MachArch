/* micro86_common:
 *
 * Functions and definitions common to micro86 and related programs.
 *
 * Note: certain functions (indicated) require dataset initialization
 * with m86ds_init() function; dataset should also be destroyed after
 * access with m86ds_kill() function to avoid memory leaks. Both of
 * these functions are declared in micro86_dataset.h.
 */

#ifndef _STDLIB_H
#include <stdlib.h>
#endif

#ifndef _STRING_H
#include <string.h>
#endif

#ifndef COMMONERR_H
#include "common/common_err.h"
#endif

#ifndef MICRO86COMMON_H
#include "micro86_common.h"
#endif

#ifndef MICRO86DATASET_H
#include "micro86_dataset.h"
#endif

/* m86_std_instruct_format: saves instruction integer in standard
 * format in string parameter.
 *
 * Parameters (in order):
 *
 * # string to save formatted instruction integer in.
 * # original integer.
 *
 * Note: string parameter must be large enough to hold
 * INSTRUCT_NUM_DIGITS characters (defined in micro86_common.h).
 *
 * Note: passing NULL for string parameter results in no operation
 * being done.
 *
 * Note: passing integers too large to convert to an
 * INSTRUCT_NUM_DIGITS digit number in base INSTRUCT_BASE (both
 * defined in micro86_common.h) results in undefined behavior.
 *
 * Returns: N/A.
 */
void m86_std_instruct_format(
        char *str,
        const int instruct)
{
    if (str == NULL) return;
    sprintf(str, "0x%08X", instruct);
    return;
}

/* m86_std_opcode_format: saves opcode integer in standard format in
 * string parameter.
 *
 * Parameters (in order):
 *
 * # string to save formatted opcode integer in.
 * # original integer.
 *
 * Note: string parameter must be large enough to hold
 * OPCODE_NUM_DIGITS characters (defined in micro86_common.h).
 *
 * Note: passing NULL for string parameter results in no operation
 * being done.
 *
 * Note: passing integers too large to convert to an OPCODE_NUM_DIGITS
 * digit number in base OPCODE_BASE (both defined in micro86_common.h)
 * results in undefined behavior.
 *
 * Returns: N/A.
 */
void m86_std_opcode_format(
        char *str,
        const int opcode)
{
    m86_std_instruct_format(str, opcode);
    return;
}

/* m86_std_operand_format: saves operand integer in standard format in
 * string parameter.
 *
 * Parameters (in order):
 *
 * # string to save formatted operand integer in.
 * # original integer.
 *
 * Note: string parameter must be large enough to hold
 * OPERAND_NUM_DIGITS characters (defined in micro86_common.h).
 *
 * Note: passing NULL for string parameter results in no operation
 * being done.
 *
 * Note: passing integers too large to convert to an
 * OPERAND_NUM_DIGITS digit number in base OPERAND_BASE (both defined
 * in micro86_common.h) results in undefined behavior.
 *
 * Returns: N/A.
 */
void m86_std_operand_format(
        char *str,
        const int operand)
{
    m86_std_instruct_format(str, operand);
    return;
}


/* m86_di_is_valid_instruct: return true if specified instruction in
 * decoded form is valid; otherwise, return false.
 *
 * Note: an instruction is considered to be valid if the opcode
 * associated with the instruction is part of the dataset.
 *
 * Note: calling this function without initializing the dataset with
 * m86ds_init() first results in a fatal error.
 *
 * Parameters (in order):
 *
 * # m86_decoded_instruct variable.
 *
 * Returns: bool value to indicate whether or not specified
 * instruction in decoded form is valid; true = yes, false = no.
 */
bool m86_di_is_valid_instruct(const m86_decoded_instruct di)
{
    if (!m86ds_is_init()) m86ds_uninit_error(stderr, EXIT_FAILURE);
    return m86ds_is_valid_opcode(di.opcode);
}

/* m86_di_instruct_has_operand: return true if specified instruction
 * in decoded form takes an operand; otheriwse return false.
 *
 * Note: calling this function without initializing the dataset with
 * m86ds_init() first results in a fatal error.
 *
 * Parameters (in order):
 *
 * # m86_decoded_instruct variable.
 *
 * Note: if instruction is not valid, this function terminates
 * calling process with a fatal error message printed to stderr,
 * passing EXIT_FAILURE error code (defined in stdlib.h) to the
 * operating system.
 *
 * Returns: bool value to indicate whether or not specified
 * instruction in decoded form takes an operand; true = yes, false =
 * no.
 */
bool m86_di_instruct_has_operand(const m86_decoded_instruct di)
{
    if (!m86ds_is_init()) m86ds_uninit_error(stderr, EXIT_FAILURE);
    return m86ds_opcode_has_operand(di.opcode);
}

/* m86_di_instruct_is_immediate: return true if specified instruction
 * in decoded form is an immediate instruction; otherwise return
 * false.
 *
 * Note: calling this function without initializing the dataset with
 * m86ds_init() first results in a fatal error.
 *
 * Parameters (in order):
 *
 * # m86_decoded_instruct variable.
 *
 * Note: if instruction is not valid, this function terminates
 * calling process with a fatal error message printed to stderr,
 * passing EXIT_FAILURE error code (defined in stdlib.h) to the
 * operating system.
 *
 * Note: if opcode is for an instruction that does not take an
 * operand, this function terminates calling process with a fatal
 * error message printed to stderr, passing EXIT_FAILURE (defined in
 * stdlib.h) to the operating system.
 *
 * Returns: bool value to indicate whether or not specified
 * instruction in decoded form is an immediate instruction; true =
 * yes, false = no.
 */

bool m86_di_instruct_is_immediate(const m86_decoded_instruct di)
{
    if (!m86ds_is_init()) m86ds_uninit_error(stderr, EXIT_FAILURE);
    return m86ds_opcode_is_immediate(di.opcode);
}

/* m86_di_encoded: return decoded instruction in encoded form.
 *
 * Parameters (in order):
 *
 * Note: this function assumes decoded instruction is in correct form
 * with OPCODE_NUM_DIGITS (defined in micro86_common.h) digits in base
 * OPCODE_BASE (defined in micro86_common.h); passing decoded
 * instructions in incorrect form results in undefined return values.
 *
 * # m86_decoded_instruct variable.
 *
 * Returns: m86_encoded_instruct value containing decoded instruction
 * in encoded form.
 */
m86_encoded_instruct m86_di_encoded(const m86_decoded_instruct di)
{
    m86_encoded_instruct ei;
    ei = (di.operand + (di.opcode << 16));
    return ei;
}

/* m86_ei_is_valid_instruct: return true if specified instruction in
 * encoded form is valid; otherwise, return false.
 *
 * Note: an instruction is considered to be valid if the opcode
 * associated with the instruction is part of the dataset.
 *
 * Note: calling this function without initializing the dataset with
 * m86ds_init() first results in a fatal error.
 *
 * Parameters (in order):
 *
 * # m86_encoded_instruct variable.
 *
 * Returns: bool value to indicate whether or not specified
 * instruction in encoded form is valid; true = yes, false = no.
 */
bool m86_ei_is_valid_instruct(const m86_encoded_instruct ei)
{
    if (!m86ds_is_init()) m86ds_uninit_error(stderr, EXIT_FAILURE);
    return m86ds_is_valid_opcode(m86_ei_decoded_opcode(ei));
}

/* m86_ei_instruct_has_operand: return true if specified instruction
 * in encoded form takes an operand; otherwise, return false.
 *
 * Note: calling this function without initializing the dataset with
 * m86ds_init() first results in a fatal error.
 *
 * Parameters (in order):
 *
 * # m86_encoded_instruct variable.
 *
 * Note: if instruction is not valid, this function terminates
 * calling process with a fatal error message printed to stderr,
 * passing EXIT_FAILURE error code (defined in stdlib.h) to the
 * operating system.
 *
 * Returns: bool value to indicate whether or not specified
 * instruction in encoded form takes an operand; true = yes, false =
 * no.
 */
bool m86_ei_instruct_has_operand(const m86_encoded_instruct ei)
{
    if (!m86ds_is_init()) m86ds_uninit_error(stderr, EXIT_FAILURE);
    return m86ds_opcode_has_operand(m86_ei_decoded_opcode(ei));
}

/* m86_ei_instruct_is_immediate: return true if specified instruction
 * in encoded form is an immediate instruction; otherwise, return
 * false.
 *
 * Note: calling this function without initializing the dataset with
 * m86ds_init() first results in a fatal error.
 *
 * Parameters (in order):
 *
 * # m86_encoded_instruct variable.
 *
 * Note: if instruction is not valid, this function terminates
 * calling process with a fatal error message printed to stderr,
 * passing EXIT_FAILURE error code (defined in stdlib.h) to the
 * operating system.
 *
 * Note: if opcode is for an instruction that does not take an
 * operand, this function terminates calling process with a fatal
 * error message printed to stderr, passing EXIT_FAILURE (defined in
 * stdlib.h) to the operating system.
 *
 * Returns: bool value to indicate whether or not specified
 * instruction in encoded form is an immediate instruction; true =
 * yes, false = no.
 */
bool m86_ei_instruct_is_immediate(const m86_encoded_instruct ei)
{
    if (!m86ds_is_init()) m86ds_uninit_error(stderr, EXIT_FAILURE);
    return m86ds_opcode_is_immediate(m86_ei_decoded_opcode(ei));
}

/* m86_ei_decoded: return encoded instruction in decoded form.
 *
 * Parameters (in order):
 *
 * # m86_encoded_instruct variable.
 *
 * Note: this function assumes encoded instruction is in correct form
 * with INSTRUCT_NUM_DIGITS (defined in micro86_common.h) digits in
 * base INSTRUCT_BASE (defined in micro86_common.h); passing encoded
 * instructions in incorrect form results in undefined return values.
 *
 * Returns: m86_ei_decoded value containing encoded instruction in
 * decoded form.
 */
m86_decoded_instruct m86_ei_decoded(const m86_encoded_instruct ei)
{
    m86_decoded_instruct di;
    di.opcode = (ei >> 16);
    di.operand  = (ei & 0x0000FFFF);
    return di;
}

/* m86_ei_decoded_opcode: return decoded opcode of encoded
 * instruction.
 *
 * Parameters (in order):
 *
 * # m86_encoded_instruct variable.
 *
 * Note: this function assumes encoded instruction is in correct form
 * with INSTRUCT_NUM_DIGITS (defined in micro86_common.h) digits in
 * base INSTRUCT_BASE (defined in micro86_common.h); passing encoded
 * instructions in incorrect form results in undefined return values.
 *
 * Returns: decoded opcode of encoded instruction.
 */
int m86_ei_decoded_opcode(const m86_encoded_instruct ei)
{
    return (ei >> 16);
}

/* m86_ei_decoded_operand: return decoded operand of encoded
 * instruction.
 *
 * Parameters (in order):
 *
 * Note: this function assumes encoded instruction is in correct form
 * with INSTRUCT_NUM_DIGITS (defined in micro86_common.h) digits in
 * base INSTRUCT_BASE (defined in micro86_common.h); passing encoded
 * instructions in incorrect form results in undefined return values.
 *
 * Returns: decoded operand of encoded instruction.
 */
int m86_ei_decoded_operand(const m86_encoded_instruct ei)
{
    return (ei & 0x0000FFFF);
}

/* m86_syntax_error: prints out a syntax error message to specified
 * file stream with specified file name and exits if error code
 * parameter is EXIT_FAILURE.
 *
 * Parameters (in order):
 *
 * # file name where syntax error is present.
 * # line number of syntax error in file.
 * # file stream to print error message to.
 * # integer for error code.
 *
 * Note: passing NULL for file name or file stream results in no
 * operation being done.
 *
 * Note: if line number parameter is <= 0, no line number is printed
 * with syntax error message.
 *
 * Note: this function only terminates the calling process if the
 * error code parameter is EXIT_FAILURE (defined in stdlib.h), in
 * which case EXIT_FAILURE is returned to the operating system.
 *
 * Returns: N/A.
 */
void m86_syntax_error(
        const char *file_name,
        const int line_number,
        FILE *stream,
        const int error_code)
{
    if ((file_name == NULL) || (stream == NULL)) return;
    if (line_number <= 0)
        fprintf(stream, "ERROR: invalid syntax in %s!\n", file_name);
    else
        fprintf(stream, "ERROR: invalid syntax in line %i in %s!\n",
                line_number, file_name);
    exit_on_exit_fail(error_code, error_code);
    return;
}

/* m86_invalid_opcode_error: prints out an invalid opcode error
 * message to specified file stream with specified opcode and exits if
 * error code parameter is EXIT_FAILURE.
 *
 * Parameters (in order):
 *
 * # file stream to print error message to.
 * # invalid opcode the error message is about.
 * # integer for error code.
 *
 * Note: passing NULL for file stream results in no operation being
 * done.
 *
 * Note: this function only terminates the calling process if the
 * error code parameter is EXIT_FAILURE (defined in stdlib.h), in
 * which case EXIT_FAILURE is returned to the operating system.
 *
 * Returns: N/A.
 */
void m86_invalid_opcode_error(
        FILE *stream,
        const int opcode,
        const int error_code)
{
    if (stream == NULL) return;
    fprintf(stream, "ERROR: opcode '0x%04X' is invalid!\n", opcode);
    exit_on_exit_fail(error_code, error_code);
    return;
}

/* m86_invalid_mnemonic_error: prints out an invalid mnemonic error
 * message to specified file stream with specified mnemonic and exits
 * if error code parameter is EXIT_FAILURE.
 *
 * Parameters (in order):
 *
 * # file stream to print error message to.
 * # invalid mnemonic the error message is about.
 * # integer for error code.to use.
 *
 * Note: passing NULL for file stream or mnemonic results in no
 * operation being done.
 *
 * Note: this function only terminates the calling process if the
 * error code parameter is EXIT_FAILURE (defined in stdlib.h), in
 * which case EXIT_FAILURE is returned to the operating system.
 *
 * Returns: N/A.
 */
void m86_invalid_mnemonic_error(
        FILE *stream,
        const char *mnemonic,
        const int error_code)
{
    if ((stream == NULL) || (mnemonic == NULL)) return;
    fprintf(stream, "ERROR: mnemonic '%s' is invalid!\n", mnemonic);
    exit_on_exit_fail(error_code, error_code);
    return;
}

/* m86_opcode_wo_operand_error: prints out an error to specified file
 * stream with specified opcode to indicate that instruction
 * associated with opcode does not take an operand; also exits if
 * error code parameter is EXIT_FAILURE.
 *
 * Parameters (in order):
 *
 * # file stream to print error message to.
 * # opcode assoicated with instruction that does not take an operand.
 * # integer for error code.
 *
 * Note: passing NULL for file stream results in no operation being
 * done.
 *
 * Note: this function only terminates the calling process if the
 * error code parameter is EXIT_FAILURE (defined in stdlib.h), in
 * which case EXIT_FAILURE is returned to the operating system.
 *
 * Returns: N/A.
 */
void m86_opcode_wo_operand_error(
        FILE *stream,
        const int opcode,
        const int error_code)
{
    if (stream == NULL) return;
    fprintf(stream, "ERROR: instruction with opcode '0x%04X' does not"
            " take an operand!\n", opcode);
    exit_on_exit_fail(error_code, error_code);
    return;
}

/* m86_menmonic_wo_operand_error: prints out an error to specified
 * file stream with specified mnemonic to indicate that instruction
 * asssociated with mnemonic does not take an operand; also exits if
 * error code parameter is EXIT_FAILURE.
 *
 * Parameters (in order):
 *
 * # file stream to print error message to.
 * # mnemonic assoicated with instruction that does not take an
 * # operand.
 * # integer for error code.
 *
 * Note: passing NULL for file stream or mnemonic results in no
 * operation being done.
 *
 * Note: this function only terminates the calling process if the
 * error code parameter is EXIT_FAILURE (defined in stdlib.h), in
 * which case EXIT_FAILURE is returned to the operating system.
 *
 * Returns: N/A.
 */
void m86_mnemonic_wo_operand_error(
        FILE *stream,
        const char *mnemonic,
        const int error_code)
{
    if ((stream == NULL) || (mnemonic == NULL)) return;
    fprintf(stream, "ERROR: instruction with mnemonic '%s' does not"
            " take an operand!\n", mnemonic);
    exit_on_exit_fail(error_code, error_code);
    return;
}

/* EOF. */

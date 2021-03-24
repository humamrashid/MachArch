/* micro86_dataset:
 *
 * Dataset for micro86 and related programs.
 *
 * WARNING: access to the dataset is not threadsafe.
 *
 * Note: most dataset functions require dataset initialization with
 * m86ds_init() function; dataset should also be destryoed after
 * access with m86ds_kill() function to avoid memory leaks.
 */

#ifndef _STDBOOL_H
#include <stdbool.h>
#endif

#ifndef MICRO86DATASET_H
#define MICRO86DATASET_H

#define M86DS_FILE_COMMENT '#'
#define M86DS_FILE_DELIM " "
#define M86DS_FILE_LINE_SIZE 80
#define M86DS_FILE_ITEM_SIZE 8
#define M86DS_FILE_NAME "micro86_data.m86db"
#define M86DS_FILE_SYNTAX "%08X = %8s %c %c"
#define M86DS_FILE_SYNTAX_OPERAND_CHAR 'o'
#define M86DS_FILE_SYNTAX_IMMEDIATE_CHAR 'i'

/* m86ds_init: initialize micro86 dataset.
 *
 * Note: this function must be called to access the dataset and
 * therefore it must be called before most other functions declared in
 * this file to avoid fatal errors.
 *
 * Note: to avoid memory leaks, m86ds_kill() should be called after
 * accessing the dataset.
 *
 * Note: this function is to be called only once, subsequent calls to
 * this function without calling m86ds_kill() first results in a fatal
 * error.
 *
 * Parameters (in order): N/A.
 *
 * Returns: N/A.
 */
void m86ds_init(void);

/* m86ds_init: return true if the dataset has been initialized;
 * otherwise return false.
 *
 * Parameters (in order): N/A.
 *
 * Returns: bool value to indicate status of dataset initialization;
 * true = initialized, false = uninitialized.
 */
bool m86ds_is_init(void);

/* m86ds_get_opcode: return opcode mapped to specifed mnemonic if
 * found; otherwise exit with a fatal error condition.
 *
 * Note: calling this function without initializing the dataset with
 * m86ds_init() first results in a fatal error.
 *
 * Parameters (in order):
 *
 * # string for mnemonic.
 *
 * Note: if mnemonic is not mapped to any opcode, this function
 * terminates the calling process with a fatal error message printed
 * to stderr, passing the EXIT_FAILURE error code (defined in
 * stdlib.h).
 *
 * Returns: opcode mapped to mnemonic.
 */
int m86ds_get_opcode(const char*);

/* m86ds_get_mnemonic: return mnemonic mapped to specifed opcode if
 * found; otherwise exit with a fatal error condition.
 *
 * Note: calling this function without initializing the dataset with
 * m86ds_init() first results in a fatal error.
 *
 * Parameters (in order):
 *
 * # integer for opcode.
 *
 * Note: opcodes are stored as maximally 8-digit, hexadecimal
 * integers.
 *
 * Note: if opcode is not mapped to any mnemonic, this function
 * terminates the calling process with a fatal error message printed
 * to stderr, passing the EXIT_FAILURE error code (defined in
 * stdlib.h).
 *
 * Returns: mnemonic mapped to opcode.
 */
char *m86ds_get_mnemonic(const int);

/* m86ds_is_valid_opcode: return true if specified opcode is valid
 * (i.e., part of the dataset); otherwise, return false.
 *
 * Note: calling this function without initializing the dataset with
 * m86ds_init() first results in a fatal error.
 *
 * Parameters (in order):
 *
 * # integer opcode to check for validity.
 *
 * Returns: bool value to indicate opcode validity; true = valid,
 * false = invalid.
 */
bool m86ds_is_valid_opcode(const int);

/* m86ds_is_valid_mnemonic: return true if specified mnemonic is valid
 * (i.e., part of the dataset); otherwise, return false.
 *
 * Note: calling this function without initializing the dataset with
 * m86ds_init() first results in a fatal error.
 *
 * Parameters (in order):
 *
 * # string mnemonic to check for validity.
 *
 * Note: by definition, if string mnmeonic parameter is NULL, it is
 * not valid and return value is false.
 *
 * Returns: bool value to indicate mnemonic validity; true = valid,
 * false = invalid.
 */
bool m86ds_is_valid_mnemonic(const char*);

/* m86ds_opcode_has_operand: return true if instruction associated
 * with specified opcode takes an operand; otherwise, return false.
 *
 * Note: calling this function without initializing the dataset with
 * m86ds_init() first results in a fatal error.
 *
 * Parameters (in order):
 *
 * # integer for opcode.
 *
 * Note: if opcode is not associated with any instruction, this
 * function terminates calling process with a fatal error message
 * printed to stderr, passing EXIT_FAILURE error code (defined in
 * stdlib.h) to the operating system.
 *
 * Returns: bool value to indicate whether or not instruction
 * associated with specified opcode takes an operand; true = yes,
 * false = no.
 */
bool m86ds_opcode_has_operand(const int);

/* m86ds_mnemonic_has_operand: return true if instruction associated
 * with specified mnemonic takes an operand; otherwise, return false.
 *
 * Note: calling this function without initializing the dataset with
 * m86ds_init() first results in a fatal error.
 *
 * Parameters (in order):
 * 
 * # string for mnemonic.
 *
 * Note: by definition, if string mnemonic parameter is NULL,
 * mnemnonic is not valid.
 *
 * Note: if mnemonic is not associated with any instruction, this
 * function terminates calling process with a fatal error message
 * printed to stderr, passing the EXIT_FAILURE error code (defined in
 * stdlib.h) to the operating system.
 *
 * Returns: bool value to indicate whether or not instruction
 * associated with specified mnemonic takes an operand; true = yes,
 * false = no.
 */
bool m86ds_mnemonic_has_operand(const char*);

/* m86ds_opcode_is_immediate: return true if instruction associated
 * with specified opcode is an immediate instruction; otherwise,
 * return false.
 *
 * Note: calling this function without initializing the dataset with
 * m86ds_init() first results in a fatal error.
 *
 * Parameters (in order):
 *
 * # integer for opcode.
 *
 * Note: if opcode is not associated with any instruction, this
 * function terminates calling process with a fatal error message
 * printed to stderr, passing EXIT_FAILURE error code (defined in
 * stdlib.h) to the operating system.
 *
 * Note: by definition, if opcode is for an instruction that does not
 * take an operand, then it is is not immediate.
 *
 * Returns: bool value to indicate whether or not instruction
 * associated with specified opcode is an immediate instruction; true
 * = yes, false = no.
 */
bool m86ds_opcode_is_immediate(const int);

/* m86ds_mnemonic_is_immediate: return true if instruction associated
 * with specified mnemonic is an immediate instruction; otherwise,
 * return false.
 *
 * Note: calling this function without initializing the dataset with
 * m86ds_init() first results in a fatal error.
 *
 * Parameters (in order):
 *
 * # string for mnemonic.
 *
 * Note: if mnemonic is not associated with any instruction, this
 * function terminates calling process with a fatal error message
 * printed to stderr, passing the EXIT_FAILURE error code (defined in
 * stdlib.h) to the operating system.
 *
 * Note: by definition, if mnemonic is for an instruction that does
 * not take an operand, then it is is not immediate.
 *
 * Returns: bool value to indicate whether or not instruction
 * associated with specified mnemonic is an immediate instruction;
 * true = yes, false = no.
 */
bool m86ds_mnemonic_is_immediate(const char*);

/* m86ds_num_instructs: return number of instructions in the dataset.
 *
 * Note: by definition, the number of instructions when the dataset is
 * uninitialized is always zero.
 *
 * Parameters (in order): N/A.
 *
 * Returns: number of instructions in the dataset.
 */
unsigned int m86ds_num_instructs(void);

/* m86ds_is_reserved_word: return true if specified word is reserved
 * by micro86 or related programs; otherwise, return false.
 *
 * Note: calling this function without initializing the dataset with
 * m86ds_init() first results in a fatal error.
 *
 * Note: a 'word' is defined to be a string without whitespace in
 * between any of the characters.
 *
 * Note: a word is only to be considered reserved if the same word is
 * also used as a mnemonic for a micro86 instruction or if the word is
 * a keyword in a micro86 related program. A word is considered to be
 * the 'same' as another word if the function strcmp() in string.h
 * returns 0 when these two words are passed in as arguments.
 *
 * Parameters (in order):
 *
 * # string for word.
 *
 * Returns: bool value to indicate whether or not specified word is
 * reserved by micro86 or related programs; true = yes, false = no.
 */
bool m86ds_is_reserved_word(const char*);

/* m86ds_get_lookahead: return lookahead for variable declarations in
 * m86Asm.
 *
 * Parameters (in order): N/A.
 *
 * Returns: lookahead for variable declarations in m86Asm.
 */
const char *m86ds_get_lookahead(void);

/* m86ds_kill: destroy the dataset (i.e., deallocate all resources
 * used for the dataset).
 *
 * Note: this function simply deallocates system resources used for
 * accessing the dataset, the dataset file itself is left untouched.
 *
 * Note: to avoid memory leaks, m86ds_kill() should be called after
 * accessing the dataset.
 *
 * Note: this function is to be called only once after having called
 * m86ds_init(); subsequent calls to this function without calling
 * m86ds_init() first results in a fatal error.
 */
void m86ds_kill(void);

/* m86ds_uninit_error: prints out a dataset uninitialized error
 * message to specified file stream and exits if error code parameter
 * is EXIT_FAILURE.
 *
 * Parameters (in order):
 *
 * # file stream to print error message to.
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
void m86ds_uninit_error(
        FILE*,
        const int);

/* m86ds_reinit_error: prints out a dataset already initialized error
 * message to specifid file stream and exits if error code parameter
 * is EXIT_FAILURE.
 *
 * Parameters (in order):
 *
 * # file stream to print error message to.
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
void m86ds_reinit_error(
        FILE*,
        const int);

#endif

/* EOF. */

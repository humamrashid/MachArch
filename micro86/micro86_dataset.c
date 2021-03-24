/* micro86_dataset:
 *
 * Dataset for micro86 and related programs.
 *
 * WARNING: access to the dataset is not threadsafe.
 */

#ifndef _STDLIB_H
#include <stdlib.h>
#endif

#ifndef _STDIO_H
#include <stdio.h>
#endif

#ifndef __G_LIB_H__
#include <glib.h>
#endif

#ifndef COMMONERR_H
#include "common/common_err.h"
#endif

#ifndef COMMONIO_H
#include "common/common_io.h"
#endif

#ifndef COMMONSTR_H
#include "common/common_str.h"
#endif

#ifndef MICRO86COMMON_H
#include "micro86_common.h"
#endif

#ifndef MICRO86DATASET_H
#include "micro86_dataset.h"
#endif

static bool initialized = false;
static GSList *o_list = NULL;
static GSList *m_list = NULL;
static GSList *oho_list = NULL;
static GSList *mho_list = NULL;
static GSList *oi_list = NULL;
static GSList *mi_list = NULL;
static GSList *reserved_words = NULL;
static GHashTable *om_table = NULL;
static GHashTable *mo_table = NULL;
static const char *keywords[] =
{
    "VAR", NULL
};

/* m86ds_load_data: load data for access.
 *
 * Note: this function is to exit with a fatal error if dataset file
 * is unreadable or enough memory cannot be allocated for instruction
 * storage.
 *
 * Note: calling this function after the dataset has already been
 * initialized results in a fatal error.
 *
 * Parameters (in order): N/A.
 *
 * Returns: N/A.
 */
static void m86ds_load_data(void)
{
    if (initialized) m86ds_reinit_error(stderr, EXIT_FAILURE);
    FILE *file = NULL;
    if ((file = fopen(M86DS_FILE_NAME, "r")) == NULL)
        file_read_error(stderr, M86DS_FILE_NAME, EXIT_FAILURE);
    unsigned int i;
    long comment_pos;
    bool found_instruct = false;
    unsigned int line_count = 0, instruct_count = 0;
    char line[M86DS_FILE_LINE_SIZE + 1],
         instruct[M86DS_FILE_LINE_SIZE + 1];
    for (i = 0; keywords[i]; i++)
        reserved_words = g_slist_append(
                reserved_words, g_strdup(keywords[i]));
    while (fgets(line, M86DS_FILE_LINE_SIZE + 1, file) != NULL)
    {
        line_count++;
        if (is_empty_line(line)) continue;
        if ((comment_pos =
                    char_pos_str(line, M86DS_FILE_COMMENT)) != -1)
        {
            if ((unsigned long) comment_pos !=
                    strspn(line, M86DS_FILE_DELIM))
            {
                found_instruct = true;
                strncpy(instruct, line, comment_pos);
                instruct[comment_pos] = '\0';
            } else found_instruct = false;
        } else
        {
            found_instruct = true;
            strcpy(instruct, line);
        }
        if (found_instruct)
        {
            instruct_count++;
            if (M86DS_DEBUG)
                fprintf(stderr, "Found an instruction at line %u.\n",
                        line_count);
            int *opcode = NULL;
            if ((opcode = malloc(sizeof(int))) == NULL)
                memory_alloc_error(stderr, EXIT_FAILURE);
            char mnemonic[M86DS_FILE_ITEM_SIZE + 1], o_char, i_char;
            int parts = sscanf(instruct, M86DS_FILE_SYNTAX,
                    opcode, mnemonic, &o_char, &i_char);
            if ((parts != 4) && (parts != 3) && (parts != 2))
                m86_syntax_error(M86DS_FILE_NAME,
                        line_count, stderr, EXIT_FAILURE);
            char *mnemonic_p = g_strdup(mnemonic);
            switch(parts)
            {
                case 4:
                    if ((o_char !=
                                M86DS_FILE_SYNTAX_OPERAND_CHAR) ||
                            (i_char !=
                             M86DS_FILE_SYNTAX_IMMEDIATE_CHAR))
                        m86_syntax_error(M86DS_FILE_NAME,
                                line_count, stderr, EXIT_FAILURE);
                    oi_list = g_slist_append(
                            oi_list,
                            opcode);
                    mi_list = g_slist_append(
                            mi_list,
                            mnemonic_p);
                case 3:
                    if (o_char != M86DS_FILE_SYNTAX_OPERAND_CHAR)
                        m86_syntax_error(M86DS_FILE_NAME,
                                line_count, stderr, EXIT_FAILURE);
                    oho_list = g_slist_append(
                            oho_list,
                            opcode);
                    mho_list = g_slist_append(
                            mho_list,
                            mnemonic_p);
                case 2:
                    o_list = g_slist_append(
                            o_list,
                            opcode);
                    m_list = g_slist_append(
                            m_list,
                            mnemonic_p);
                    reserved_words = g_slist_append(
                            reserved_words,
                            mnemonic_p);
                    g_hash_table_insert(
                            om_table,
                            opcode,
                            mnemonic_p);
                    g_hash_table_insert(
                            mo_table,
                            mnemonic_p,
                            opcode);
                    break;
            }
            if (M86DS_DEBUG)
            {
                char std_form[OPCODE_NUM_DIGITS];
                m86_std_opcode_format(std_form, *opcode);
                fprintf(stderr, "Instruction read: %s\n"
                        "Opcode read: %s, mnemonic read: %s\n",
                        instruct, std_form, mnemonic);
            }
        }
    }
    if (M86DS_DEBUG)
        fprintf(stderr, "Total instructions read: %u\n",
                instruct_count);
    return;
}

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
void m86ds_init(void)
{
    if (initialized) m86ds_reinit_error(stderr, EXIT_FAILURE);
    else
    {
        om_table = g_hash_table_new(g_int_hash, g_str_equal);
        mo_table = g_hash_table_new(g_str_hash, g_str_equal);
        m86ds_load_data();
        initialized = true;
    }
    return;
}

/* m86ds_init: return true if the dataset has been initialized;
 * otherwise return false.
 *
 * Parameters (in order): N/A.
 *
 * Returns: bool value to indicate status of dataset initialization;
 * true = initialized, false = uninitialized.
 */
bool m86ds_is_init(void)
{
    return initialized;
}

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
int m86ds_get_opcode(const char *key)
{
    if (!initialized) m86ds_uninit_error(stderr, EXIT_FAILURE);
    int *value = g_hash_table_lookup(mo_table, key);
    if (value == NULL)
        m86_invalid_mnemonic_error(stderr, key, EXIT_FAILURE);
    return *value;
}

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
char *m86ds_get_mnemonic(const int key)
{
    if (!initialized) m86ds_uninit_error(stderr, EXIT_FAILURE);
    char *value = g_hash_table_lookup(om_table, &key);
    if (value == NULL)
        m86_invalid_opcode_error(stderr, key, EXIT_FAILURE);
    return value;
}

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
bool m86ds_is_valid_opcode(const int opcode)
{
    if (!initialized) m86ds_uninit_error(stderr, EXIT_FAILURE);
    return g_hash_table_contains(om_table, &opcode);
}

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
bool m86ds_is_valid_mnemonic(const char* mnemonic)
{
    if (!initialized) m86ds_uninit_error(stderr, EXIT_FAILURE);
    return (mnemonic == NULL) ? false :
        g_hash_table_contains(mo_table, mnemonic);
}

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
bool m86ds_opcode_has_operand(const int opcode)
{
    if (!initialized) m86ds_uninit_error(stderr, EXIT_FAILURE);
    if (!m86ds_is_valid_opcode(opcode))
        m86_invalid_opcode_error(stderr, opcode, EXIT_FAILURE);
    int *d = NULL;
    GSList *i = NULL;
    for (i = oho_list; i; i = i->next)
    {
        d = i->data;
        if (*d == opcode) return true;
    }
    return false;
}

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
bool m86ds_mnemonic_has_operand(const char *mnemonic)
{
    if (!initialized) m86ds_uninit_error(stderr, EXIT_FAILURE);
    if (!m86ds_is_valid_mnemonic(mnemonic))
        m86_invalid_mnemonic_error(stderr, mnemonic, EXIT_FAILURE);
    GSList *g = g_slist_find_custom(
            mho_list, mnemonic, (GCompareFunc) strcmp);
    return (g == NULL) ? false : true;
}

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
bool m86ds_opcode_is_immediate(const int opcode)
{
    if (!initialized) m86ds_uninit_error(stderr, EXIT_FAILURE);
    if (!m86ds_opcode_has_operand(opcode)) return false;
    int *d = NULL;
    GSList *i = NULL;
    for (i = oi_list; i; i = i->next)
    {
        d = i->data;
        if (*d == opcode) return true;
    }
    return false;
}

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
bool m86ds_mnemonic_is_immediate(const char *mnemonic)
{
    if (!initialized) m86ds_uninit_error(stderr, EXIT_FAILURE);
    if (!m86ds_mnemonic_has_operand(mnemonic)) return false;
    GSList *g = g_slist_find_custom(
            mi_list, mnemonic, (GCompareFunc) strcmp);
    return (g == NULL) ? false : true;
}

/* m86ds_num_instructs: return number of instructions in the dataset.
 *
 * Note: by definition, the number of instructions when the dataset is
 * uninitialized is always zero.
 *
 * Parameters (in order): N/A.
 *
 * Returns: number of instructions in the dataset.
 */
unsigned int m86ds_num_instructs(void)
{
    return (initialized) ? g_hash_table_size(om_table) : 0;
}

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
bool m86ds_is_reserved_word(const char *word)
{
    if (!initialized) m86ds_uninit_error(stderr, EXIT_FAILURE);
    if (str_has_char(word, ' ')) return false;
    GSList *g = g_slist_find_custom(
            reserved_words, word, (GCompareFunc) strcmp);
    return (g == NULL) ? false : true;
}

/* m86ds_get_lookahead: return lookahead for variable declarations in
 * m86Asm.
 *
 * Parameters (in order): N/A.
 *
 * Returns: lookahead for variable declarations in m86Asm.
 */
const char *m86ds_get_lookahead(void)
{
    return keywords[0];
}

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
void m86ds_kill(void)
{
    if (initialized)
    {
        unsigned int i;
        for (i = 0; keywords[i]; i++)
            reserved_words = g_slist_delete_link(
                    reserved_words,
                    g_slist_find_custom(
                        reserved_words,
                        keywords[i],
                        (GCompareFunc) strcmp));
        g_slist_free_full(o_list, g_free);
        g_slist_free_full(m_list, g_free);
        initialized = false;
    } else m86ds_uninit_error(stderr, EXIT_FAILURE);
    return;
}

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
        FILE* stream,
        const int error_code)
{
    if (stream == NULL) return;
    fprintf(stream, "ERROR: dataset not initialized!\n");
    exit_on_exit_fail(error_code, error_code);
    return;
}

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
        FILE* stream,
        const int error_code)
{
    if (stream == NULL) return;
    fprintf(stream, "ERROR: dataset already initialized!\n");
    exit_on_exit_fail(error_code, error_code);
    return;
}

/* EOF. */

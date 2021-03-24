/* micro86:
 *
 * An emulator for a simplified model of the Intel 8086 processor.
 */

#ifndef _STDLIB_H
#include <stdlib.h>
#endif

#ifndef MICRO86PROC_H
#include "micro86_proc.h"
#endif

#ifndef MEMORY_H
#include "memory/memory.h"
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

#ifndef MICRO86_H
#include "micro86.h"
#endif

/* Print out contents of memory.
 */
static void m86_print_memory(
        const memory micro86_memory,
        const unsigned int size,
        FILE *stream)
{
    if (stream == NULL) return;
    fprintf(stream, "\nMEMORY:\n\n");
    m_print_memory(micro86_memory, 0, size, stream);
    return;
}

/* Print out contents of cpu.
 */
static void m86_print_cpu(
        const micro86_proc micro86_cpu,
        FILE *stream)
{
    if (stream == NULL) return;
    fprintf(stream, "\nCPU:\n\n");
    m86_print_proc(micro86_cpu, stream);
    return;
}

/* Print out contents of cpu and memory.
 */
static void m86_postmortem_dump(
        const micro86_proc micro86_cpu,
        const memory micro86_memory,
        const unsigned int size,
        FILE *stream)
{
    fprintf(stream, "\n=== POST-MORTEM DUMP ===\n");
    m86_print_cpu(micro86_cpu, stream);
    m86_print_memory(micro86_memory, size, stream);
    return;
}

/* Print out error message parameter, post-mortem dump and exit.
 */
static void m86_error(
        FILE *stream,
        const char *message,
        const int error_code,
        const micro86_proc micro86_cpu,
        const memory micro86_memory,
        const unsigned int mem_size)
{
    fprintf(stream, "%s\n", message);
    m86_postmortem_dump(micro86_cpu,
            micro86_memory, mem_size, stream);
    exit_on_exit_fail(error_code, error_code);
    return;
}

/* Check if divisor is zero and cause a fatal error if it is.
 */
static void m86_check_zero_div_error(
        int divisor,
        FILE *stream,
        const int error_code,
        const micro86_proc micro86_cpu,
        const memory micro86_memory,
        const unsigned int mem_size)
{
    if (divisor == 0)
        m86_error(stream, "Micro86 ERROR: division by zero!",
                error_code, micro86_cpu, micro86_memory, mem_size);
    return;
}

/* Check if there is a program in memory and cause a fatal error if
 * there isn't.
 */
static void m86_check_no_prgm_error(
        const unsigned int program_size,
        FILE *stream,
        const int error_code,
        const micro86_proc micro86_cpu,
        const memory micro86_memory,
        const unsigned int mem_size)
{
    if (program_size == 0)
        m86_error(stream, "Micro86 ERROR: no program in memory!",
                error_code, micro86_cpu, micro86_memory, mem_size);
    return;
}

/* Check memory bounds and cause a fatal error if violated.
 */
static void m86_check_memory_bounds(
        const int position,
        FILE *stream,
        const int error_code,
        const micro86_proc micro86_cpu,
        const memory micro86_memory,
        const unsigned int mem_size)
{
    if ((position < 0) || ((unsigned) position > mem_size))
    {
        memory_bounds_error(STD_ERR_DEST, position, 0);
        m86_error(stream, "Micro86 ERROR: memory violation!",
                error_code, micro86_cpu, micro86_memory, mem_size);
    }
    return;
}

/* Set the zero and sign bits of the flag register of the cpu.
 */
static void m86_set_flag_reg(
        micro86_proc *micro86_cpu,
        const int value)
{
    if (value == 0)
    {
        m86_set_flags_zb(micro86_cpu, true);
        m86_set_flags_sb(micro86_cpu, false);
    } else if (value < 0)
    {
        m86_set_flags_sb(micro86_cpu, true);
        m86_set_flags_zb(micro86_cpu, false);
    } else
    {
        m86_set_flags_zb(micro86_cpu, false);
        m86_set_flags_sb(micro86_cpu, false);
    }
    return;
}

/* Return true if instruction argument is a jump instruction;
 * otherwise, return false.
 */
static bool is_jmp_instruct(const m86_decoded_instruct di)
{
    switch (di.opcode)
    {
        case JMPI:
        case JEI:
        case JNEI:
        case JLI:
        case JLEI:
        case JGI:
        case JGEI:  return true;
        default:    break;
    }
    return false;
}

/* Print out disassembled instruction. */
static void m86_disassemble(
        FILE *stream,
        const micro86_proc micro86_cpu,
        const memory micro86_memory,
        const unsigned int mem_size,
        const int word)
{
    bool has_operand = false,
         nonjmp_immediate = false;
    const m86_decoded_instruct di = m86_ei_decoded(word);
    char *output = malloc(sizeof(char) * M86_PRINT_FORMAT_SIZE);
    if (m86_di_is_valid_instruct(di))
    {
        output = m86ds_get_mnemonic(di.opcode);
        if (m86_di_instruct_has_operand(di)) has_operand = true;
        if (m86_di_instruct_is_immediate(di) &&
                !(is_jmp_instruct(di))) nonjmp_immediate = true;
    } else sprintf(output, M86_PRINT_FORMAT, word);
    if (has_operand)
    {
        m86_check_memory_bounds(di.operand, STD_ERR_DEST,
                EXIT_FAILURE, micro86_cpu, micro86_memory, mem_size);
        if (nonjmp_immediate)
            fprintf(stream, "%s\t\t"
                    M86_PRINT_FORMAT "\n", output, di.operand);
        else
            fprintf(stream, "%s\t\t" M86_PRINT_FORMAT "\t\t|"
                    M86_PRINT_FORMAT ": " M86_PRINT_FORMAT "|\n",
                    output, di.operand, di.operand,
                    m_get_value(micro86_memory, di.operand));
    } else fprintf(stream, "%s\n", output);
    return;
}

/* Print out disassembled code in memory.
 */
static void m86_disassembly(
        FILE *stream,
        const micro86_proc micro86_cpu,
        const memory micro86_memory,
        const unsigned int mem_size,
        const unsigned int program_size)
{
    m86_check_no_prgm_error(program_size, STD_ERR_DEST,
            EXIT_FAILURE, micro86_cpu, micro86_memory, mem_size);
    unsigned int i;
    fprintf(stream, "\n=== DISASSEMBLED CODE ===\n\n");
    for (i = 0; i < program_size; i++)
    {
        m86_check_memory_bounds(i, STD_ERR_DEST,
                EXIT_FAILURE, micro86_cpu, micro86_memory, mem_size);
        fprintf(stream, M86_PRINT_FORMAT ":\t", i);
        m86_disassemble(stream, micro86_cpu,
                micro86_memory, mem_size,
                m_get_value(micro86_memory, i));
    }
    return;
}

/* Execute the decoded instruction.
 */
static void execute(
        FILE *stream,
        bool *running,
        const bool trace,
        micro86_proc *micro86_cpu,
        memory *micro86_memory,
        const unsigned int mem_size,
        const m86_decoded_instruct di)
{
    if (trace)
    {
        fprintf(stream, M86_PRINT_FORMAT ":\t",
                m86_get_ip_reg(*micro86_cpu) - 1);
        m86_disassemble(stream, *micro86_cpu,
                *micro86_memory, mem_size,
                m86_get_ir_reg(*micro86_cpu));
        fprintf(stream, "\t\t");
        m86_print_proc(*micro86_cpu, stream);
    }
    switch (di.opcode)
    {
        case HALT:  *running = false;
                    break;
        case LOAD:  m86_check_memory_bounds(di.operand, STD_ERR_DEST,
                            EXIT_FAILURE,
                            *micro86_cpu, *micro86_memory, mem_size);
                    m86_set_acc_reg(micro86_cpu,
                            m_get_value(*micro86_memory, di.operand));
                    break;
        case LOADI: m86_set_acc_reg(micro86_cpu, di.operand);
                    break;
        case STORE: m86_check_memory_bounds(di.operand, STD_ERR_DEST,
                            EXIT_FAILURE,
                            *micro86_cpu, *micro86_memory, mem_size);
                    m_set_value(micro86_memory, di.operand,
                            m86_get_acc_reg(*micro86_cpu));
                    break;
        case ADD:   m86_check_memory_bounds(di.operand, STD_ERR_DEST,
                            EXIT_FAILURE,
                            *micro86_cpu, *micro86_memory, mem_size);
                    m86_set_acc_reg(micro86_cpu,
                            m86_get_acc_reg(*micro86_cpu)
                            + m_get_value(*micro86_memory,
                                di.operand));
                    break;
        case ADDI:  m86_set_acc_reg(micro86_cpu,
                            m86_get_acc_reg(*micro86_cpu)
                            + di.operand);
                    break;
        case SUB:   m86_check_memory_bounds(di.operand, STD_ERR_DEST,
                            EXIT_FAILURE,
                            *micro86_cpu, *micro86_memory, mem_size);
                    m86_set_acc_reg(micro86_cpu,
                            m86_get_acc_reg(*micro86_cpu)
                            - m_get_value(*micro86_memory,
                                di.operand));
                    break;
        case SUBI:  m86_set_acc_reg(micro86_cpu,
                            m86_get_acc_reg(*micro86_cpu)
                            - di.operand);
                    break;
        case MUL:   m86_check_memory_bounds(di.operand, STD_ERR_DEST,
                            EXIT_FAILURE,
                            *micro86_cpu, *micro86_memory, mem_size);
                    m86_set_acc_reg(micro86_cpu,
                            m86_get_acc_reg(*micro86_cpu)
                            * m_get_value(*micro86_memory,
                                di.operand));
                    break;
        case MULI:  m86_set_acc_reg(micro86_cpu,
                            m86_get_acc_reg(*micro86_cpu)
                            * di.operand);
                    break;
        case DIV:   m86_check_memory_bounds(di.operand, STD_ERR_DEST,
                            EXIT_FAILURE,
                            *micro86_cpu, *micro86_memory, mem_size);
                    {
                        int divisor =
                            m_get_value(*micro86_memory, di.operand);
                        m86_check_zero_div_error(divisor,
                                STD_ERR_DEST, EXIT_FAILURE,
                                *micro86_cpu, *micro86_memory,
                                mem_size);
                        m86_set_acc_reg(micro86_cpu,
                                m86_get_acc_reg(*micro86_cpu)
                                / divisor);
                    }
                    break;
        case DIVI:  m86_check_zero_div_error(di.operand, STD_ERR_DEST,
                            EXIT_FAILURE, *micro86_cpu,
                            *micro86_memory, mem_size);
                    m86_set_acc_reg(micro86_cpu,
                            m86_get_acc_reg(*micro86_cpu)
                            / di.operand);
                    break;
        case MOD:   m86_check_memory_bounds(di.operand, STD_ERR_DEST,
                            EXIT_FAILURE,
                            *micro86_cpu, *micro86_memory, mem_size);
                    {
                        int divisor =
                            m_get_value(*micro86_memory, di.operand);
                        m86_check_zero_div_error(divisor,
                                STD_ERR_DEST, EXIT_FAILURE,
                                *micro86_cpu, *micro86_memory,
                                mem_size);
                        m86_set_acc_reg(micro86_cpu,
                                m86_get_acc_reg(*micro86_cpu)
                                % divisor);
                    }
                    break;
        case MODI:  m86_check_zero_div_error(di.operand, STD_ERR_DEST,
                            EXIT_FAILURE, *micro86_cpu,
                            *micro86_memory, mem_size);
                    m86_set_acc_reg(micro86_cpu,
                            m86_get_acc_reg(*micro86_cpu)
                            % di.operand);
                    break;
        case CMP:   m86_check_memory_bounds(di.operand, STD_ERR_DEST,
                            EXIT_FAILURE,
                            *micro86_cpu, *micro86_memory, mem_size);
                    m86_set_flag_reg(micro86_cpu,
                            m86_get_acc_reg(*micro86_cpu)
                            - m_get_value(*micro86_memory,
                                di.operand));
                    break;
        case CMPI:  m86_set_flag_reg(micro86_cpu,
                        m86_get_acc_reg(*micro86_cpu) - di.operand);
                    break;
        case JMPI:  m86_set_ip_reg(micro86_cpu, di.operand);
                    break;
        case JEI:   if (m86_get_flags_zb(*micro86_cpu) ==
                            ZERO_BIT_TRUE)
                        m86_set_ip_reg(micro86_cpu, di.operand);
                    break;
        case JNEI:  if (m86_get_flags_zb(*micro86_cpu) ==
                            ZERO_BIT_FALSE)
                        m86_set_ip_reg(micro86_cpu, di.operand);
                    break;
        case JLI:   if (m86_get_flags_sb(*micro86_cpu) ==
                            SIGN_BIT_TRUE)
                        m86_set_ip_reg(micro86_cpu, di.operand);
                    break;
        case JLEI:  if ((m86_get_flags_sb(*micro86_cpu) ==
                                SIGN_BIT_TRUE) ||
                            (m86_get_flags_zb(*micro86_cpu) ==
                             ZERO_BIT_TRUE))
                        m86_set_ip_reg(micro86_cpu, di.operand);
                    break;
        case JGI:   if ((m86_get_flags_zb(*micro86_cpu) ==
                                ZERO_BIT_FALSE) &&
                            (m86_get_flags_sb(*micro86_cpu) ==
                             SIGN_BIT_FALSE))
                        m86_set_ip_reg(micro86_cpu, di.operand);
                    break;
        case JGEI:  {
                        unsigned int zero_bit =
                            m86_get_flags_zb(*micro86_cpu);
                        if (((zero_bit == ZERO_BIT_FALSE) &&
                                    (m86_get_flags_sb(*micro86_cpu) ==
                                     SIGN_BIT_FALSE)) || (zero_bit ==
                                     ZERO_BIT_TRUE))
                            m86_set_ip_reg(micro86_cpu, di.operand);
                    }
                    break;
        case IN:    {
                        int input = fgetc(STD_IN_SRC);
                        if (input == EOF)
                        {
                            file_read_error(STD_ERR_DEST,
                                    "'STD_IN_SRC'", 0);
                            m86_error(STD_ERR_DEST,
                                    "Micro86 ERROR:"
                                    " cannot read input!",
                                    EXIT_FAILURE, *micro86_cpu,
                                    *micro86_memory, mem_size);
                        }
                        m86_set_acc_reg(micro86_cpu,
                                (unsigned char) input);
                    }
                    break;
        case OUT:   fprintf(STD_OUT_DEST, "%c\n",
                            (unsigned char)
                            m86_get_acc_reg(*micro86_cpu));
                    break;
        default:    m86_invalid_opcode_error(STD_ERR_DEST, di.opcode,
                            0);
                    m86_error(STD_ERR_DEST,
                            "Micro86 ERROR: invalid instruction!",
                            EXIT_FAILURE, *micro86_cpu,
                            *micro86_memory, mem_size);
                    break;
    }
    return;
}

/* Fetch the instruction from memory and return a decoded version.
 */
static m86_decoded_instruct fetch(
        micro86_proc *micro86_cpu,
        const memory micro86_memory,
        const unsigned int mem_size,
        const unsigned int program_size)
{
    m86_check_no_prgm_error(program_size, STD_ERR_DEST,
            EXIT_FAILURE, *micro86_cpu, micro86_memory, mem_size);
    unsigned int ip = m86_get_ip_reg(*micro86_cpu);
    if (ip >= mem_size)
    {
        memory_bounds_error(STD_ERR_DEST, ip, 0);
        m86_error(STD_ERR_DEST, "Micro86 ERROR: memory violation!",
                EXIT_FAILURE, *micro86_cpu, micro86_memory, mem_size);
    }
    m86_set_ip_reg(micro86_cpu, (ip + 1));
    if (m86_get_ip_reg(*micro86_cpu) > program_size)
        m86_error(STD_ERR_DEST,
                "Micro86 ERROR: program end reached!",
                EXIT_FAILURE, *micro86_cpu, micro86_memory, mem_size);
    m86_set_ir_reg(micro86_cpu, m_get_value(micro86_memory, ip));
    return m86_ei_decoded(m86_get_ir_reg(*micro86_cpu));
}

/* Boot up the emulator and run the FDE cycle.
 */
static void m86_boot_up(
        FILE *stream,
        const char *file_name,
        micro86_proc *micro86_cpu,
        memory *micro86_memory,
        const unsigned int mem_size,
        const unsigned int program_size,
        const bool dump,
        const bool trace,
        bool *running)
{
    *running = true;
    fprintf(stream, "*** Micro86 Emulator V. " M86_VERSION_NUM
            " BOOTING ***\n\n" "Program file: %s\n", file_name); 
    if (trace) fprintf(stream, "\n=== EXECUTION TRACE ===\n\n");
    while (*running)
    {
        execute(
                stream,
                running,
                trace,
                micro86_cpu,
                micro86_memory,
                mem_size,
                fetch(
                    micro86_cpu,
                    *micro86_memory,
                    mem_size,
                    program_size));
    }
    if (dump)
        m86_disassembly(stream, *micro86_cpu,
                *micro86_memory, mem_size, program_size);
    m86_postmortem_dump(*micro86_cpu,
            *micro86_memory, mem_size, stream);
    fprintf(stream, "\n*** Micro86 Emulator V. " M86_VERSION_NUM
            " HALTED ***\n");
    return;
}

/* Load the program into memory.
 */
static void m86_loader(
        const char *file_name,
        const micro86_proc micro86_cpu,
        memory *micro86_memory,
        unsigned int *mem_size,
        const bool mem_resize,
        unsigned int *program_size)

{
    *program_size = 0;
    FILE *file = NULL;
    if ((file = fopen(file_name, "r")) == NULL)
    {
        file_read_error(STD_ERR_DEST, file_name, 0);
        m86_error(STD_ERR_DEST, "Micro86 ERROR:"
                " cannot read program file!",
                EXIT_FAILURE, micro86_cpu,
                *micro86_memory, *mem_size);
    }
    long comment_pos;
    bool found_instruct = false;
    unsigned int line_count = 0, instruct_count = 0;
    char line[M86_PRGM_FILE_LINE_SIZE + 1],
         instruct[M86_PRGM_FILE_LINE_SIZE + 1];
    while (fgets(line, M86_PRGM_FILE_LINE_SIZE + 1, file) != NULL)
    {
        line_count++;
        if (is_empty_line(line)) continue;
        if ((comment_pos =
                    char_pos_str(line, M86_PRGM_FILE_COMMENT)) != -1)
        {
            if ((unsigned long) comment_pos !=
                    strspn(line, M86_PRGM_FILE_DELIM))
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
            if (M86_DEBUG)
                fprintf(STD_ERR_DEST,
                        "Found an instruction at line %u.\n",
                        line_count);
            int instruction;
            if (sscanf(instruct, M86_PRGM_SYNTAX, &instruction) != 1)
            {
                m86_syntax_error(file_name,
                        line_count, STD_ERR_DEST, 0);
                m86_error(STD_ERR_DEST, "Micro86 ERROR:"
                        " invalid instruction!", EXIT_FAILURE,
                        micro86_cpu, *micro86_memory, *mem_size);
            }
            if (M86_DEBUG)
            {
                char std_form[INSTRUCT_NUM_DIGITS];
                m86_std_instruct_format(std_form, instruction);
                fprintf(STD_ERR_DEST, "Instruction read: %s\n",
                        std_form);
            }
            if (*program_size >= *mem_size)
            {
                if (!mem_resize)
                {
                    memory_bounds_error(STD_ERR_DEST, *program_size,
                            0);
                    m86_error(STD_ERR_DEST,
                            "Micro86 ERROR: memory violation!",
                            EXIT_FAILURE, micro86_cpu,
                            *micro86_memory, *mem_size);
                } else
                {
                    m_extend_init(micro86_memory, *mem_size,
                            M86_MEM_EXT_SIZE, M86_INIT_MEM_VAL);
                    *mem_size += M86_MEM_EXT_SIZE;
                }
            }
            m_set_value(micro86_memory,
                    (*program_size)++, instruction);
        }
    }
    if (M86_DEBUG)
        fprintf(STD_ERR_DEST, "Total instructions read: %u\n",
                instruct_count);
    return;
}

/* Process the command-line arguments.
 */
static const char *m86_process_cmd_line(
        const int argc,
        char *argv[],
        bool *dump,
        bool *trace,
        bool *mem_resize)
{
    if ((argc < 2) || (argc > 5)) return NULL;
    int i;
    char *file_name = NULL;
    bool file_found = false;
    for (i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            char *opt = argv[i] + 1;
            if (!(strcmp(opt, M86_DUMP_OPT))) *dump = true;
            else if (!(strcmp(opt, M86_MEM_RESIZE_OPT)))
                *mem_resize = true;
            else if (!(strcmp(opt, M86_TRACE_OPT))) *trace = true;
            else return NULL;
        } else
        {
            if (file_found) return NULL;
            file_name = argv[i];
            file_found = true;
        }
    }
    return file_name;
}

int main(int argc, char *argv[])
{
    bool dump = false,
         trace = false,
         mem_resize = false,
         running = false;
    micro86_proc micro86_cpu;
    m86_proc_init(&micro86_cpu);
    memory micro86_memory;
    unsigned int mem_size = M86_DEF_MEM_SIZE;
    m_allocate_init(&micro86_memory, mem_size, M86_INIT_MEM_VAL);
    const char *file_name;
    if ((file_name = m86_process_cmd_line(argc, argv,
                    &dump, &trace, &mem_resize)) == NULL)
    {
        fprintf(STD_ERR_DEST,
                "Usage: %s <program_file> [-"
                M86_DUMP_OPT " (dump)] [-"
                M86_MEM_RESIZE_OPT " (memory resize)] [-"
                M86_TRACE_OPT " (trace)]\n", argv[0]);
        m86_error(STD_ERR_DEST, "Micro86 ERROR:"
                " unable to set up environment!",
                EXIT_FAILURE, micro86_cpu, micro86_memory, mem_size);
    }
    unsigned int program_size;
    m86_loader(file_name, micro86_cpu,
            &micro86_memory, &mem_size, mem_resize, &program_size);
    m86ds_init();
    m86_boot_up(STD_OUT_DEST, file_name, &micro86_cpu,
            &micro86_memory, mem_size, program_size,
            dump, trace, &running);
    m86ds_kill();
    m_deallocate(&micro86_memory);
    return EXIT_SUCCESS;
}

/* EOF. */

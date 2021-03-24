/* micro86:
 *
 * An emulator for a simplified model of the Intel 8086 processor.
 */

#ifndef MICRO86_H
#define MICRO86_H

/* M86_VERSION_NUM: current version of emulator.
 */
#define M86_VERSION_NUM "1.0"

/* M86_DUMP_OPT: command-line dump option.
 */
#define M86_DUMP_OPT "d"

/* M86_TRACE_OPT: command-line execution trace option.
 */
#define M86_TRACE_OPT "t"

/* M86_MEM_RESIZE_OPT: command-line memory resize option.
 */
#define M86_MEM_RESIZE_OPT "r"

/* M86_DEF_MEM_SIZE: default memory size (i.e., number of memory units
 * allocated initially).
 */
#define M86_DEF_MEM_SIZE 20

/* M86_INIT_MEM_VAL: initial value for all memory units.
 */
#define M86_INIT_MEM_VAL 0x00

/* M86_PRGM_FILE_LINE_SIZE: maximum size of line in program file to run on
 * emulator.
 */
#define M86_PRGM_FILE_LINE_SIZE 80

/* M86_PRGM_FILE_COMMENT: comment indicator for emulator program file.
 */
#define M86_PRGM_FILE_COMMENT '#'

/* M86_PRGM_FILE_DELIM: delimiter for emulator program file syntax.
 */
#define M86_PRGM_FILE_DELIM " "

/* M86_PRGM_SYNTAX: instruction syntax for emulator programs.
 */
#define M86_PRGM_SYNTAX "%08X"

/* M86_PRINT_FORMAT: format for emulator memory address, instruction
 * and related print outs.
 */
#define M86_PRINT_FORMAT "0x%08X"

/* M86_PRINT_FORMAT_SIZE: size length in number of characters requited
 * to print values in M86_PRINT_FORMAT.
 */
#define M86_PRINT_FORMAT_SIZE 11

/* M86_MEM_EXT_SIZE: extension size for memory (used only if memory is
 * resizeable).
 */
#define M86_MEM_EXT_SIZE M86_DEF_MEM_SIZE

/* Instruction set opcodes.
 */
#define HALT    0x0100
#define LOAD    0x0202
#define LOADI   0x0201
#define STORE   0x0302
#define ADD     0x0402
#define ADDI    0x0401
#define SUB     0x0502
#define SUBI    0x0501
#define MUL     0x0602
#define MULI    0x0601
#define DIV     0x0702
#define DIVI    0x0701
#define MOD     0x0802
#define MODI    0x0801
#define CMP     0x0902
#define CMPI    0x0901
#define JMPI    0x0A01
#define JEI     0x0B01
#define JNEI    0x0C01
#define JLI     0x0D01
#define JLEI    0x0E01
#define JGI     0x0F01
#define JGEI    0x1001
#define IN      0x1100
#define OUT     0x1200

/* STD_IN_SRC: standard input source for emulator programs.
 */
#define STD_IN_SRC stdin

/* STD_OUT_SRC: standard output destination for emulator programs.
 */
#define STD_OUT_DEST stdout

/* STD_ERR_DEST: standard error destination for emulator.
 */
#define STD_ERR_DEST stderr

#endif

/* EOF. */

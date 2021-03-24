/* Micro86DataSet
 *
 * Data set for Micro86 related programs.
 *
 * Author:      Humam Rashid
 *
 * Assignment:  03-M86Asm
 *
 * Class:       CISC-3160, Programming Languages
 */

import java.io.*;
import java.util.*;

public class Micro86DataSet {

    // Opcodes to mnemonics table.
    private Map<Integer, String> opcodeTable = null;

    // Mnemonics to opcodes table.
    private Map<String, Integer> mnemonicTable = null;

    // Opcodes to 'has operand' table.
    private Map<Integer, Boolean> opcodeHasOperand = null;

    // Mnemonics to 'has operand' table.
    private Map<String, Boolean> mnemonicHasOperand = null;

    // Special keywords.
    private final String[] keywords = {"VAR"};

    // Reserved words.
    private List<String> reservedWords = null;

    // Opcode list.
    private List<Integer> opcodeList = null;

    // Immediate opcodes list.
    private List<Integer> immediateOpcodes = null;

    // Mnemonic list.
    private List<String> mnemonicList = null;

    // Immediate mnemonics list.
    private List<String> immediateMnemonics = null;

    // File read from.
    private String dataFileName = "micro86Data.m86db";

    /* Return integer word in standard instruction format. */
    public String stdInstructFormat(int word) {
        return String.format("%08X", word);
    }

    /* Return integer word in binary instruction format. */
    public String binaryInstructFormat(int word) {
        return (String.format("%32s",
                    Integer.toBinaryString(word)).replace(' ', '0'));
    }

    /* Class for objects representing a decoded instruction. */
    public class DecodedInstructionFormat {
        private int opcode;
        private int operand;
        /* Initialize a decoded instruction object with specified
         * opcode and operand. */
        public DecodedInstructionFormat(int opcode, int operand) {
            this.opcode = opcode;
            this.operand = operand;
            return;
        }
        /* Return opcode. */
        public int getOpcode() {
            return opcode;
        }
        /* Return operand. */
        public int getOperand() {
            return operand;
        }
        /* Set opcode. */
        public void setOpcode(int opcode) {
            this.opcode = opcode;
            return;
        }
        /* Set operand. */
        public void setOperand(int operand) {
            this.operand = operand;
            return;
        }
        /* Return String representation of decoded instruction object.
         * */
        public String toString() {
            return ("Opcode: " + opcode + " "
                    + "Operand: " + operand);
        }
        /* Return encoded form of decoded instruction in standard
         * instruction format. */
        public String encoded() {
            return (opcodeHasOperand.get(opcode)) ?
                stdInstructFormat((opcode << 16) | operand) :
                stdInstructFormat((opcode << 16) | 0x00000000);
        }
        /* Return encoded form of decoded instruction in binary
         * instruction format. */
        public String binaryEncoded() {
            return (opcodeHasOperand.get(opcode)) ?
                binaryInstructFormat((opcode << 16) | operand) :
                binaryInstructFormat((opcode << 16) | 0x00000000);
        }
    }

    /* Class for objects representing an encoded instruction. */
    public class EncodedInstructionFormat {
        private int instructCode;
        /* Initialize encoded instruction object with specified
         * instruction code. */
        public EncodedInstructionFormat(int instructCode) {
            this.instructCode = instructCode;
            return;
        }
        /* Return instruction code. */
        public int getInstructionCode() {
            return instructCode;
        }
        /* Set instruction code. */
        public void setInstructionCode(int instructCode) {
            this.instructCode = instructCode;
        }
        /* Return String representation of encoded instruction object.
         * */
        public String toString() {
            return ("Instruction code: " + instructCode);
        }
        /* Return decoded form of instruction code split into opcode
         * and operand. */
        public int[] decoded() {
            return new int[]{
                instructCode >>> 16, instructCode & 0x0000FFFF
            };
        }
    }

    /* Initialize maps/tables and lists. */
    public Micro86DataSet() {

        opcodeTable = new HashMap<Integer, String>();
        mnemonicTable = new HashMap<String, Integer>();
        opcodeHasOperand = new HashMap<Integer, Boolean>();
        mnemonicHasOperand = new HashMap<String, Boolean>();
        opcodeList = new ArrayList<Integer>();
        immediateOpcodes = new ArrayList<Integer>();
        mnemonicList = new ArrayList<String>();
        immediateMnemonics = new ArrayList<String>();

        loadTables();

        reservedWords = new ArrayList<String>();
        for (String s: mnemonicList) reservedWords.add(s);
        for (int i = 0; i < keywords.length; i++)
            reservedWords.add(keywords[i]);

        return;
    }

    /* Get opcode associated with mnemonic.
     *
     * Throws IllegalArgumentException if mnemonic is invalid. */
    public int getOpcode(String mnemonic) {
        if (!mnemonicList.contains(mnemonic))
            throw new IllegalArgumentException("Invalid mnemonic");
        return mnemonicTable.get(mnemonic);
    }

    /* Get mnemonic associated with opcode.
     *
     * Throws IllegalArgumentException if opcode is invalid. */
    public String getMnemonic(int opcode) {
        if (!opcodeList.contains(opcode))
            throw new IllegalArgumentException("Invalid opcode!");
        return opcodeTable.get(opcode);
    }

    /* Returns true if opcode is valid. */
    public boolean isValidOpcode(int opcode) {
        return opcodeList.contains(opcode);
    }

    /* Returns true if opcode has operand.
     *
     * Throws IllegalArgumentException if opcode is invalid. */
    public boolean opcodeHasOperand(int opcode) {
        if (!opcodeList.contains(opcode))
            throw new IllegalArgumentException("Invalid opcode!");
        return opcodeHasOperand.get(opcode);
    }

    /* Returns true if mnemonic is valid. */
    public boolean isValidMnemonic(String mnemonic) {
        return mnemonicList.contains(mnemonic);
    }

    /* Returns true if mnemonic has operand.
     *
     * Throws IllegalArgumentException if mnemonic is invalid. */
    public boolean mnemonicHasOperand(String mnemonic) {
        if (!mnemonicList.contains(mnemonic))
            throw new IllegalArgumentException("Invalid mnemonic!");
        return mnemonicHasOperand.get(mnemonic);
    }

    /* Returns true if opcode is immediate.
     *
     * Throws IllegalArgumentException if opcode is invalid. */
    public boolean isImmediateOpcode(int opcode) {
        if (!opcodeList.contains(opcode))
            throw new IllegalArgumentException("Invalid opcode!");
        return immediateOpcodes.contains(opcode);
    }

    /* Returns true if mnemonic is immediate.
     *
     * Throws IllegalArgumentException if mnemonic is invalid. */
    public boolean isImmediateMnemonic(String mnemonic) {
        if (!mnemonicList.contains(mnemonic))
            throw new IllegalArgumentException("Invalid mnemonic!");
        return immediateMnemonics.contains(mnemonic);
    }
    
    /* Load the tables and lists. */
    private void loadTables() {
        int lineCount = 0;
        Scanner dataFile = null;
        String text = null;
        String tokens[] = null;
        try {
            dataFile = new Scanner(new BufferedReader(
                        new FileReader(dataFileName)));
            while (dataFile.hasNextLine()) {
                lineCount++;
                text = dataFile.nextLine();
                if (text.startsWith("#") || text.isEmpty())
                    continue;
                tokens = text.split("\\s+");
                if (!((tokens.length == 3
                                || (tokens.length == 4
                                && tokens[3].equals("o"))
                                || (tokens.length == 5
                                && tokens[3].equals("o")
                                && tokens[4].equals("i")))
                                && tokens[1].equals("="))) {
                    syntaxError(lineCount, dataFileName);
                }
                String mnemonic = tokens[2];
                int opcode = Integer.parseInt(tokens[0], 16);
                opcodeTable.put(opcode, mnemonic);
                opcodeList.add(opcode);
                mnemonicTable.put(mnemonic, opcode);
                mnemonicList.add(mnemonic);
                switch(tokens.length) {
                    case 5:
                        immediateOpcodes.add(opcode);
                        immediateMnemonics.add(mnemonic);
                    case 4:
                        opcodeHasOperand.put(opcode, true);
                        mnemonicHasOperand.put(mnemonic, true);
                        break;
                    default:
                        opcodeHasOperand.put(opcode, false);
                        mnemonicHasOperand.put(mnemonic, false);
                        break;
                }
            }
        } catch(IOException ioe) {
            System.out.println(ioe.getMessage());
            System.exit(1);
        } catch(NumberFormatException nfe) {
            syntaxError(lineCount, dataFileName);
        } finally {
            if (dataFile != null) dataFile.close();
        }
        return;
    }

    /* Returns true if word is reserved. */
    public boolean isReservedWord(String word) {
        return reservedWords.contains(word);
    }
 
    /* Return lookahead for variable declarations. */
    public String getDeclrLookahead() {
        return keywords[0];
    }

    /* Indicate syntax error by line and file name. */
    private void syntaxError(int line, String fileName) {
        System.err.printf("Invalid syntax in line %d in %s!%n",
                line, fileName);
        System.exit(2);
        return;
    }

}

// EOF.

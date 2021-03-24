/* M86Asm
 *
 * A rudimentary assembler for Micro86 instructions.
 *
 * Author:      Humam Rashid
 *
 * Assignment:  03-M86Asm
 *
 * Class:       CISC-3160, Programming Languages
 */

import java.io.*;
import java.util.*;
import java.text.*;

public class M86Asm {

    private boolean showData = false;
    private Micro86DataSet dataSet = new Micro86DataSet();
    private boolean m86Instruct = false, cppCode = false;
    private final String m86Comment = ";", labelDelim = ":";

    /* Class for objects representing intermediate code. */
    private class IntermediateCode {
        private int opcode;
        private String operand;
        /* Initialize intermediate code object. */
        public IntermediateCode(int opcode, String operand) {
            this.opcode = opcode;
            this.operand = operand;
            return;
        }
        /* Return opcode. */
        public int getOpcode() {
            return opcode;
        }
        /* Return operand. */
        public String getOperand() {
            return operand;
        }
        /* Set opcode. */
        public void setOpcode(int opcode) {
            this.opcode = opcode;
            return;
        }
        /* Set operand. */
        public void setOperand(String operand) {
            this.operand = operand;
            return;
        }
        /* Return String representation of intermediate code. */
        public String toString() {
            return ("Opcode: " + opcode + " "
                    + "Operand: " + operand);
        }
    }

    /* Class for representing a translation into Micro86 instructions.
     * */
    private class Micro86Translation {
        private int position = 1;
        private Map<String, Integer> labels = null,
                variables = null, varPositions = null;
        private List<IntermediateCode> instructions = null;
        /* Initialize maps/tables and lists. */
        public Micro86Translation() {
            labels = new TreeMap<String, Integer>();
            variables = new TreeMap<String, Integer>();
            varPositions = new TreeMap<String, Integer>();
            instructions = new ArrayList<IntermediateCode>();
            return;
        }
        /* Initialize maps/tables and lists and add one set of
         * translation elements. */
        public Micro86Translation(String name,
                int value, IntermediateCode instruction) {
            this();
            addVariable(name, value);
            addInstruction(instruction);
            return;
        }
        /* Return label name to label position table. */
        public Map<String, Integer> getLabels() {
            return labels;
        }
        /* Return variable name to variable value table. */
        public Map<String, Integer> getVariables() {
            return variables;
        }
        /* Return variable name to variable position table. */
        public Map<String, Integer> getVarPositions() {
            return varPositions;
        }
        /* Add a label with specified name and position. */
        public void addLabel(String name, int value) {
            labels.put(name, value);
            return;
        }
        /* Add a variable with specified name and value. */
        public void addVariable(String name, int value) {
            variables.put(name, value);
            varPositions.put(name, position++);
            return;
        }
        /* Return list of instructions. */
        public List<IntermediateCode> getInstructions() {
            return instructions;
        }
        /* Add an instruction. */
        public void addInstruction(IntermediateCode instruction) {
            instructions.add(instruction);
            return;
        }
        /* Return number of instructions in translation. */
        public int numInstructions() {
            return instructions.size();
        }
        /* Return number of variables in translation. */
        public int numVariables() {
            return variables.size();
        }
        /* Return number of labels in translation. */
        public int numLabels() {
            return labels.size();
        }
        @Override
        /* Return String representation of translation. */
        public String toString() {
            String values = "";
            if (instructions.size() > 0) {
                values += "Instructions:\n";
                for (IntermediateCode ic : instructions) {
                    values += "\n"
                        + ic.getOpcode() + " " + ic.getOperand();
                }
                values += "\n";
            }
            if (variables.size() > 0) {
                values += "\nVariable values:\n";
                for (Map.Entry<String,
                        Integer> e : variables.entrySet()) {
                    values += "\n" + e.getKey() + " " + e.getValue();
                }
            }
            if (varPositions.size() > 0) {
                values += "\nVariable positions:\n";
                for (Map.Entry<String,
                        Integer> e : varPositions.entrySet()) {
                    values += "\n" + e.getKey() + " " + e.getValue();
                }
            }
            if (labels.size() > 0) {
                values += "\nLabel positions:\n";
                for (Map.Entry<String,
                        Integer> e : labels.entrySet()) {
                    values += "\n" + e.getKey() + " " + e.getValue();
                }
            }
            return values;
        }

    }

    /* Class for representing a translation to C++ code. */
    private class CPPTranslation {
        private List<String[]> operations = null;
        private Map<Integer, String> labels = null;
        private Map<String, Integer> variables = null;
        /* Initialize maps/tables and lists. */
        public CPPTranslation() {
            operations = new ArrayList<String[]>();
            labels = new TreeMap<Integer, String>();
            variables = new TreeMap<String, Integer>();
            return;
        }
        /* Initialize maps/tables and lists and add one set of
         * translation elements. */
        public CPPTranslation(String name,
                int value, String[] operation) {
            this();
            addVariable(name, value);
            addOperation(operation);
            return;
        }
        /* Return label position to label name table. */
        public Map<Integer, String> getLabels() {
            return labels;
        }
        /* Return variable name to variable position table. */
        public Map<String, Integer> getVariables() {
            return variables;
        }
        /* Add a label with specified position and name. */
        public void addLabel(int value, String name) {
            labels.put(value, name);
            return;
        }
        /* Add a variable with specified name and value. */
        public void addVariable(String name, int value) {
            variables.put(name, value);
            return;
        }
        /* Return list of operations. */
        public List<String[]> getOperations() {
            return operations;
        }
        /* Add an operation. */
        public void addOperation(String[] operation) {
            operations.add(operation);
            return;
        }
        /* Return number of operations in translation. */
        public int numOperations() {
            return operations.size();
        }
        /* Return number of variables in translation. */
        public int numVariables() {
            return variables.size();
        }
        /* Return number of labels in translation. */
        public int numLabels() {
            return labels.size();
        }
        @Override
        /* Return String representation of translation. */
        public String toString() {
            String values = "";
            if (operations.size() > 0) {
                values += "Operations:\n";
                for (String[] op: operations) {
                    if (op.length == 1) values += "\n" + op[0];
                    else values += "\n" + op[0] + " " + op[1];
                }
                values += "\n";
            }
            if (variables.size() > 0) {
                values += "\nVariable values:\n";
                for (Map.Entry<String,
                        Integer> e : variables.entrySet()) {
                    values += "\n" + e.getKey() + " " + e.getValue();
                }
            }
            if (labels.size() > 0) {
                values += "\nLabel positions:\n";
                for (Map.Entry<Integer,
                        String> e : labels.entrySet()) {
                    values += "\n" + e.getKey() + " " + e.getValue();
                }
            }
            return values;
        }
    }

    /* Write Micro86 instructions to specified destination. */
    private void writeM86Code(
            Micro86Translation translation,
            PrintStream dest, String[] fileNames, long started) {
        String metaData = "", output = "";
        List<IntermediateCode> instructList =
            translation.getInstructions();
        Map<String, Integer>
            labelTable = translation.getLabels(),
            varTable = translation.getVariables(),
            varPositionsTable = translation.getVarPositions();
        final int
            varCount = translation.numVariables(),
            labelCount = translation.numLabels(),
            instructCount = translation.numInstructions();
        Micro86DataSet.DecodedInstructionFormat dif =
            dataSet.new DecodedInstructionFormat(0, 0);
        metaData += "# Micro86 instructions.%n"
            + "# Assembled using M86Asm.%n"
            + "# Dated: "
            + new SimpleDateFormat("HH:mm:ss, MM/dd/yyyy").format(
                    Calendar.getInstance().getTime())
            + ".%n# Approx. assembling time: "
            + (System.currentTimeMillis() - started)
            + " ms.%n# Number of instructions: "
            + instructCount
            + ".%n# Number of memory units allocated: "
            + varCount
            + ".%n%n# === CODE === #%n%n";
        for (IntermediateCode ic: instructList) {
            dif.setOpcode(ic.getOpcode());
            String operand = ic.getOperand();
            if (operand != null) {
                if (dataSet.isImmediateOpcode(dif.getOpcode())) {
                    try {
                        dif.setOperand(Integer.parseInt(operand));
                    } catch(NumberFormatException nfe) {
                        if (!labelTable.containsKey(operand))
                            syntaxError(fileNames[0]);
                        dif.setOperand(labelTable.get(operand));
                    }
                } else {
                    if (!varTable.containsKey(operand))
                        syntaxError(fileNames[0]);
                    dif.setOperand((instructCount
                                + varPositionsTable.get(
                                    operand)) - 1);
                }
            }
            output += dif.encoded() + "%n";
        }
        if (varCount > 0) {
            for (Map.Entry<String, Integer> e: varTable.entrySet()) {
                output +=
                    dataSet.stdInstructFormat(e.getValue()) + "%n";
            }
        }
        if (!showData) dest.printf(output);
        else dest.printf(metaData + output + "%n# === EOF === #%n");
        if (fileNames[1] != null) {
            System.out.printf("Micro86 instructions written to: %s%n",
                    fileNames[1]);
        }
        return;
    }

    /* Write C++ code to specified destination. */
    private void writeCPPCode(CPPTranslation translation,
            PrintStream dest, String[] fileNames, long started) {
        String output = "", metaData= "";
        Map<Integer, String> labelTable = translation.getLabels();
        Map<String, Integer> varTable = translation.getVariables();
        List<String[]> operationList = translation.getOperations();
        final int
            varCount = translation.numVariables(),
            labelCount = translation.numLabels(),
            operationCount = translation.numOperations();
        metaData += "// C++ code.%n"
            + "// Translated using M86Asm.%n"
            + "// Dated: "
            + new SimpleDateFormat("HH:mm:ss, MM/dd/yyyy").format(
                    Calendar.getInstance().getTime())
            + ".%n// Approx. translating time: "
            + (System.currentTimeMillis() - started)
            + " ms.%n// Number of operations: "
            + operationCount
            + ".%n// Number of memory units allocated: "
            + varCount
            + ".%n%n// === CODE === //%n%n";
        output += "#include <iostream>%n#include <cstdlib>%n"
            + "using namespace std;%n%nint main() {%n"
            + "int _acc = 0, _cmp;%n"
            + "char _byte;%n"
            + "cin >> noskipws;%n";
        output += "%n"
            + "// === Declarations === %n%n";
        if (varCount > 0) {
            for (Map.Entry<String, Integer> e: varTable.entrySet()) {
                output += "int "
                    + e.getKey() + " = " + e.getValue() + ";%n";
            }
        }
        output += "%n"
            + "// === Operations === %n%n";
        int iPos;
        for (String[] op: operationList) {
            iPos = Integer.parseInt(op[2]);
            if (labelTable.containsKey(iPos))
                output += labelTable.get(iPos) + ": ";
            switch (dataSet.getOpcode(op[0])) {
                case 0x0100:
                    output += "exit(0);%n";
                    break;
                case 0x0201:
                case 0x0202:
                    output += "_acc = " + op[1] + ";%n";
                    break;
                case 0x0302:
                    output += "" + op[1] + " = " + "_acc;%n";
                    break;
                case 0x401:
                case 0x402:
                    output += "_acc += " + op[1] + ";%n";
                    break;
                case 0x0501:
                case 0x0502:
                    output += "_acc -= " + op[1] + ";%n";
                    break;
                case 0x0601:
                case 0x0602:
                    output += "_acc *= " + op[1] + ";%n";
                    break;
                case 0x0701:
                case 0x0702:
                    output += "_acc /= " + op[1] + ";%n";
                    break;
                case 0x0801:
                case 0x0802:
                    output += "_acc %%= " + op[1] + ";%n";
                    break;
                case 0x0901:
                case 0x0902:
                    output += "_cmp = _acc - " + op[1] + ";%n";
                    break;
                case 0x0A01:
                    output += "goto " + op[1] + ";%n";
                    break;
                case 0x0B01:
                    output += "if (_cmp == 0) goto " + op[1] + ";%n";
                    break;
                case 0x0C01:
                    output += "if (_cmp != 0) goto " + op[1] + ";%n";
                    break;
                case 0x0D01:
                    output += "if (_cmp < 0) goto " + op[1] + ";%n";
                    break;
                case 0x0E01:
                    output += "if (_cmp <= 0) goto " + op[1] + ";%n";
                    break;
                case 0x0F01:
                    output += "if (_cmp > 0) goto " + op[1] + ";%n";
                    break;
                case 0x1001:
                    output += "if (_cmp >= 0) goto " + op[1] + ";%n";
                    break;
                case 0x1100:
                    output += "cin >> _byte;%n_acc = (int) _byte;%n";
                case 0x1200:
                    output += "cout << (char) _acc;%n";
                    break;
            }
        }
        output += "%n// === HALT Bypassed === %n%nexit(1);%n}%n";
        if (!showData) dest.printf(output);
        else dest.printf(metaData + output + "%n// === EOF === //%n");
        if (fileNames[1] != null) {
            System.out.printf("C++ code written to: %s%n",
                    fileNames[1]);
        }
        return;
    }

    /* Return a parse to micro86 instructions. */
    private Micro86Translation parseM86(String code) {
        int iPos = 0;
        String label = null;
        String[] tokens = code.split("\\s+");
        Micro86Translation translation = new Micro86Translation();
        for (int i = 0; i < tokens.length; i++) {
            if (tokens[i].endsWith(labelDelim)) {
                label = tokens[i].substring(
                        0, tokens[i].length() - 1);
                if (!validIdentifier(label)) return null;
                translation.addLabel(label, iPos);
                i++;
            }
            if (dataSet.isValidMnemonic(tokens[i])) {
                if (dataSet.mnemonicHasOperand(tokens[i])) {
                    translation.addInstruction(new IntermediateCode(
                                dataSet.getOpcode(tokens[i]),
                                tokens[i + 1]));
                    i++;
                } else {
                    translation.addInstruction(new IntermediateCode(
                                dataSet.getOpcode(tokens[i]), null));
                }
                iPos++;
            } else {
                if (tokens[i].equals(dataSet.getDeclrLookahead())) {
                    if (validIdentifier(tokens[i + 1])) {
                        translation.addVariable(tokens[i + 1],
                                Integer.parseInt(tokens[i + 2]));
                    } else {
                        return null;
                    }
                    i += 2;
                } else {
                    return null;
                }
            }
        }
        return translation;
    }

    /* Return a parse to C++ code. */
    private CPPTranslation parseCPP(String code) {
        int iPos = 0;
        String label = null;
        String[] tokens = code.split("\\s+");
        CPPTranslation translation = new CPPTranslation();
        for (int i = 0; i < tokens.length; i++) {
            if (tokens[i].endsWith(labelDelim)) {
                label = tokens[i].substring(
                        0, tokens[i].length() - 1);
                if (!validIdentifier(label)) return null;
                translation.addLabel(iPos, label);
                i++;
            }
            if (dataSet.isValidMnemonic(tokens[i])) {
                if (dataSet.mnemonicHasOperand(tokens[i])) {
                    translation.addOperation(
                            new String[]{tokens[i], tokens[i + 1],
                                String.valueOf(iPos)});
                    i++;
                } else {
                    translation.addOperation(
                            new String[]{tokens[i], null,
                                String.valueOf(iPos)});
                }
                iPos++;
            } else {
                if (tokens[i].equals(dataSet.getDeclrLookahead())) {
                    if (validIdentifier(tokens[i + 1])) {
                        translation.addVariable(tokens[i + 1],
                                Integer.parseInt(tokens[i + 2]));
                    } else {
                        return null;
                    }
                    i += 2;
                } else {
                    return null;
                }
            }
        }
        return translation;
    }

    /* Read assembly source and write out translation in Micro86
     * instructions and/or C++ code. */
    public void readSource(String[] fileNames) {
        Scanner input = null;
        PrintStream dest = null;
        try {
            input = new Scanner(new BufferedReader(
                        new FileReader(fileNames[0])));
            if (fileNames[1] != null)
                dest = new PrintStream(fileNames[1]);
            else dest = System.out;
            String text = "", code = "";
            while (input.hasNext()) {
                text = input.next();
                if (text.startsWith(m86Comment))
                    text = input.nextLine();
                else code += text + " ";
            }
            if (code.isEmpty()) return;
            long started;
            if (m86Instruct) {
                Micro86Translation m86Translation;
                started = System.currentTimeMillis();
                if ((m86Translation = parseM86(code)) == null)
                    syntaxError(fileNames[0]);
                writeM86Code(m86Translation,
                        dest, fileNames, started);
            }
            if (cppCode) {
                CPPTranslation cppTranslation;
                started = System.currentTimeMillis();
                if ((cppTranslation = parseCPP(code)) == null)
                    syntaxError(fileNames[0]);
                writeCPPCode(cppTranslation,
                        dest, fileNames, started);
            }
        } catch(IOException ioe) {
            System.out.println(ioe.getMessage());
            System.exit(2);
        } finally {
            if (input != null) input.close();
            if (fileNames[1] != null && dest != null)
                dest.close();
        }
        return;
    }

    /* Process commandline arguments. */
    public String[] processCMDLine(String[] args) {
        boolean error = false;
        String inFileName = null, outFileName = null;
        for (String arg : args) {
            if (arg.startsWith("-")) {
                if (arg.substring(1).startsWith("in=")) {
                    inFileName = arg.substring(4);
                } else if (arg.substring(1).startsWith("out=")) {
                    outFileName = arg.substring(5);
                } else if (arg.substring(1).equals("M86")) {
                    m86Instruct = true;
                } else if (arg.substring(1).equals("C++")) {
                    cppCode = true;
                } else if (arg.substring(1).equals("data")) {
                    showData = true;
                } else {
                    error = true;
                }
            } else {
                error = true;
            }
        }
        if (inFileName == null
                || !(m86Instruct || cppCode) || error) {
            System.err.printf("Usage: M86Asm <-M86 | -C++>"
                    + " <-in=in_file> [-out=out_file | -data]%n");
            System.exit(1);
        }
        return (new String[]{inFileName, outFileName});
    }

    /* Verify validity of identifier.
     * Same rules as Java with reserved words for Micro86. */
    private boolean validIdentifier(String name) {
        if (dataSet.isReservedWord(name) ||
                !Character.isJavaIdentifierStart(name.charAt(0)))
            return false;
        for (int i = 1; i < name.length(); i++) {
            if (!Character.isJavaIdentifierPart(name.charAt(i)))
                return false;
        }
        return true;
    }

    /* Indicate syntax error by file name. */
    private void syntaxError(String fileName) {
        System.err.printf("Invalid syntax in %s!%n", fileName);
        System.exit(3);
        return;
    }

    public static void main(String[] args) {
        M86Asm asm = new M86Asm();
        asm.readSource(asm.processCMDLine(args));
        return;
    }

}

// EOF.

# code-translator
# Project Structure

**Code Translator** is a compiler-like tool that validates and translates code written in a custom abstract language into **ARM64 assembly**. It performs three main phases: **syntax validation**, **semantic analysis**, and **code generation**, resulting in a valid ARM64 program.

---

## 📌 Project Objective

This tool:
1. **Validates** whether the input program follows the correct syntax of a given abstract language.
2. **Checks semantics** like label definitions and subroutine calls.
3. **Translates** the validated code into **ARM64 assembly** instructions.
4. **Outputs** the ARM64 program into a file named `arm_prog.txt`.

---

## ⚙️ Phases of Execution

### 🧾 Phase 1: Syntax Validation
- The input code (in the abstract language) is checked for **correct syntax**.
- The logic is implemented in `validation.c`.
- Syntax checking includes:
  - Instruction formats
  - Operand count and types
  - Valid keywords and symbols
- Arrays are used internally to store intermediate representations of valid instructions.

### 🔍 Phase 2: Semantic Checking
- Ensures that the **labels** and **subroutine calls** used in the code are defined.
- Checks for:
  - Undefined labels
  - Invalid jumps or branches
  - Duplicate definitions
- Only if all semantic checks pass does the tool proceed to code generation.

### ⚙️ Phase 3: Code Translation (Code Generation)
- Validated and semantically correct abstract code is converted to **ARM64 assembly**.
- ARM64 code is written to a file: `arm_prog.txt`


---

## 🧰 Technologies and concepts Used

| Tool/Language     | Purpose                                  |
|-------------------|------------------------------------------|
| 🖥️ C Programming  | Core logic for validation and translation |
| 📚 Arrays & Structs | Data storage during parsing and checking |
| 🧠 Custom Parser   | Interprets abstract language instructions |
| 🧾 ARM64 Assembly | Target architecture for translated code  |
| 🛠️ GCC / Make     | Compilation and build automation         |
| 📝 Text Files     | Input (`input.txt`) and output (`arm_prog.txt`) management |



#ifndef D814E86E_6F32_4D61_B970_E93757D187C4
#define D814E86E_6F32_4D61_B970_E93757D187C4

#include <cstdint>
#include <array>

enum // Registers
{
    // R0-R7: General Purpose Registers
    R_R0 = 0,
    R_R1 = 0,
    R_R2 = 0,
    R_R3 = 0,
    R_R4 = 0,
    R_R5 = 0,
    R_R6 = 0,
    R_R7 = 0,
    R_PC, // Program Counter (PC)
    R_COND, // Condition Code Register (CCR)
    R_COUNT
};

//  set the PC to starting position
//  0x3000 is the default
enum 
{ 
    PC_START = 0x3000
};

enum // Condition flags
{
    FL_POS = 1u << 0u, /* P */
    FL_ZRO = 1u << 1u, /* Z */
    FL_NEG = 1u << 2u, /* N */
};

enum // Opcodes
{
    OP_ADD  = 0b0001,   // add  */
    OP_AND  = 0b0101,   // bitwise and */
    OP_NOT  = 0b1001,   // bitwise not */
    OP_BR   = 0b0000,   // branch */
    OP_JMP  = 0b1100,   // jump */
    // OP_RET = OP_JMP
    OP_JSR  = 0b0100,   // jump register */
    // OP_JSRR = OP_JSR
    OP_RTI  = 0b1000,   // unused */
    OP_LD   = 0b0010,   // load */
    OP_LDI  = 0b1010,   // load indirect */
    OP_LDR  = 0b0110,   // load register */
    OP_LEA  = 0b1110,   // load effective address */
    OP_ST   = 0b0011,   // store */
    OP_STI  = 0b1011,   // store indirect */
    OP_STR  = 0b0111,   // store register */
    OP_TRAP = 0b1111,   // execute trap */
    OP_RES  = 0b1101    // reserved (unused) */
};

enum // Trap codes
{
    TRAP_GETC = 0x30,  /* get character from keyboard, not echoed onto the terminal */
    TRAP_OUT = 0x21,   /* output a character */
    TRAP_PUTS = 0x22,  /* output a word string */
    TRAP_IN = 0x23,    /* get character from keyboard, echoed onto the terminal */
    TRAP_PUTSP = 0x24, /* output a byte string */
    TRAP_HALT = 0x25   /* halt the program */
};

inline const unsigned int MEMORY_MAX = 1 << 16;
inline const unsigned int NUM_REGISTERS = R_COUNT;
typedef uint16_t Register;
typedef uint16_t ConditionFlag;
typedef uint16_t Opcode;
typedef uint16_t TrapCode;
inline std::array<uint16_t, NUM_REGISTERS> register_storage; // Register Storage
inline std::array<uint16_t, MEMORY_MAX> memory_storage; // Memory - 65536 locations


void setup_vm();


///
/// @brief 
/// @param path 
/// @return True if the read was success;   false if failed
///
bool read_image(char * path);

///
/// @brief Reads a value from a location in memory_storage.
///
/// Returns the value of `memory_storage[address]`.
///
/// @param address   The memory_storage address to read from
/// @return          The value stored at `address`
///
uint16_t mem_read(uint16_t address);

///
/// @brief Writes a value to a location in memory_storage.
///
/// Sets the value of `memory_storage[address]`.
///
/// @param address   The memory_storage address/index to write to
///
void mem_write(uint16_t address, uint16_t value);

///
/// @brief Extends the given signed integer to 16 bits.
///
/// @param x            The value to extend
/// @param bit_count    The bit width of `x`
/// @return             `x` extended to 16 bits
///
uint16_t sign_extend(uint16_t x, int bit_count);

///
/// @brief Updates the condition register.
///
/// Sets the value in `register_storage[R_COND]` based on the value in `register_storage[reg]`.
/// Positive    -> FL_POS,
/// Zero        -> FL_ZRO,
/// Negative    -> FL_NEG,
///
/// @param reg The register to get the value from. 
///
void update_flags(Register reg);

///
/// ADD+: Addition
/// 
/// Assembler Format:
///  ADD DR, SR1, SR2
///  ADD DR, SR1, imm5
///
/// Encoding:
///  register:
///  15 12   11 9    8  6    5    4 3    2 0
///  0001    DR..    SR1.    0    00.    SR2
///  immediate value
///  15 12   11 9    8  6    5    4  0
///  0001    DR..    SR1.    1    imm5
///
void op_add(uint16_t instruction);

///
/// AND+: Bitwise Logical AND
/// 
/// Assembler Format:
///  AND DR, SR1, SR2
///  AND DR, sR1, imm5
///
/// Encoding:
///  register:
///  15 12   11 9    8  6    5    4 3    2 0
///  0001    DR..    SR1.    0    00.    SR2
///  immediate value:
///  15 12   11 9    8  6    5    4  0
///  0001    DR..    SR1.    1    imm5
///
void op_and(uint16_t instruction);

///
/// NOT+: Bitwise Complement (Bitwise Logical NOT)
///
/// Assembler Format:
///  NOT SR, DR  
///
/// Encoding:
///  15 12   11 9    8  6    5    4  0
///  1001    DR..    SR..    1    1111
///
void op_not(uint16_t instruction);

///
/// BR: Conditional Branch
/// 
/// Assembler Formats:
///  BRn   LABEL 
///  BRzp  LABEL
///  BRz   LABEL 
///  BRnp  LABEL
///  BRp   LABEL 
///  BRnz  LABEL
///  BR    LABEL 
///  BRnzp LABEL
///
/// Encoding:
///  15 12   11   10   9    8       0
///  0000    n    z    p    PCoffset9
///
void op_br(uint16_t instruction);

///
/// JMP: Jump
/// RET: Return from Subroutine
///  
/// Assembler Formats:
///  JMP BaseR
///  RET
///
/// Encoding:
///  JMP:
///  15 12    11 9    8   6    5    0
///  1100     000.    BaseR    000000
///
///  RET:
///  15 12    11 9    8 6    5    0
///  1100     000.    111    000000
///
void op_jmp(uint16_t instruction);

///
/// JSR: Jump to Subroutine
/// JSRR
/// 
/// Assembler Formats:
///  JSR LABEL
///  JSR BaseR
///
/// Encoding:
///  JSR:
///  15 12   11   10       0
///  0100    1    PCoffset11
///  JSRR:
///  15 12   11   10 9    8   6    5    0    
///  0100    0    00..    BaseR    000000
/// 
void op_jsr(uint16_t instruction);

/// 
/// LD+: Load
/// 
/// Assembler Format:
///  LD DR, LABEL
/// 
/// Encoding:
///  15 12   11 9    8       0
///  0010    DR..    PCoffset9
///
void op_ld(uint16_t instruction);

/// 
/// LDI+: Load Indirect
/// 
/// Assembler Format:
///  LDI DR, LABEL
/// 
/// Encoding:
///  15 12   11 9    8       0
///  1010    DR..    PCoffset9
///
void op_ldi(uint16_t instruction);

/// 
/// LDR+: Load Base+offset
/// 
/// Assembler Format:
///  LDR DR, BaseR, offset6
/// 
/// Encoding:
/// 15 12   11 9    8   6    5     0
/// 0110    DR..    BaseR    offset6
///
void op_ldr(uint16_t instruction);

/// 
/// LEA+: Load Effective Address
/// 
/// Assembler Format:
///  LEA DR, LABEL
///
/// Encoding:
///  15 12   11 9    8       0
///  1110    DR..    PCoffset9
/// 
void op_lea(uint16_t instruction);

///
/// ST: Store
///
/// Assembler Format:
///  ST SR, LABEL
///
/// Encoding:
///  15 12   11 9    8       0
///  0011    SR..    PCoffset9
///
void op_st(uint16_t instruction);

///
/// Store Indirect
///
/// Assembler Format:
///  STI SR, LABEL  
///
/// Encoding:
///  15 12   11 9    8       0
///  1011    SR..    PCoffset9
///
void op_sti(uint16_t instruction);

///
/// Store Base+offset
///
/// Assembler Format:
///  STR SR, BaseR, offset6  
///
/// Encoding
///  15 12   11 9    8   6    5     0
///  0111    SR..    BaseR    offset6
///
void op_str(uint16_t instruction);

///
/// TRAP - System Call
/// 
/// Assembler Format:
///    
///
/// Encoding
///  15 12   11 8    7       0
///  1111    0000    trapvect8
///
void op_trap(uint16_t instruction);

///
/// Reserved
///
/// Encoding:
///  15 12
///  1101
///
void op_reserved(uint16_t instruction);

///
/// RTI: Return from Interrupt
///
/// Assembler Format:
///  RTI
///
/// Encoding:
///  15 12   11         0
///  1000    000000000000
///
void op_rti(uint16_t instruction);

void op_invalid(uint16_t instruction);


#endif // D814E86E_6F32_4D61_B970_E93757D187C4
#ifndef D814E86E_6F32_4D61_B970_E93757D187C4
#define D814E86E_6F32_4D61_B970_E93757D187C4

#include "lc3_precompiled.h"


namespace lc3 {

typedef uint16_t lc3_size_t;
typedef uint8_t register_t;
inline constexpr uint32_t PC_START = 0x3000;
inline constexpr uint32_t MEMORY_MAX = 1 << 16;
inline constexpr uint32_t NUM_REGISTERS = 10;

enum /* RegisterFiles */ {
    // R0-R7: General Purpose lc3_size_ts
    R_R0        = 0,
    R_R1        = 1,
    R_R2        = 2,
    R_R3        = 3,
    R_R4        = 4,
    R_R5        = 5,
    R_R6        = 6,
    R_R7        = 7,
    R_PC, // Program Counter (PC)
    R_COND, // Condition Code lc3_size_t (CCR)
};

enum /* Condition Flags */ {
    FL_POS      = 1 << 0, /* P */
    FL_ZRO      = 1 << 1, /* Z */
    FL_NEG      = 1 << 2, /* N */
};

enum /* Opcodes */ { 
    OP_ADD      = 0b0001,   // add 
    OP_AND      = 0b0101,   // bitwise and
    OP_NOT      = 0b1001,   // bitwise not
    OP_BR       = 0b0000,   // branch
    OP_JMP      = 0b1100,   // jump : m_op_RET
    OP_JSR      = 0b0100,   // jump register : m_op_JSRR
    OP_RTI      = 0b1000,   // unused
    OP_LD       = 0b0010,   // load
    OP_LDI      = 0b1010,   // load indirect
    OP_LDR      = 0b0110,   // load register
    OP_LEA      = 0b1110,   // load effective address
    OP_ST       = 0b0011,   // store
    OP_STI      = 0b1011,   // store indirect
    OP_STR      = 0b0111,   // store register
    OP_TRAP     = 0b1111,   // execute trap
    OP_RES      = 0b1101    // reserved (unused)
};

enum /* Trap codes */ {
    TRAP_GETC   = 0x20,  // get character from keyboard, not echoed onto the terminal
    TRAP_OUT    = 0x21,   // output a character
    TRAP_PUTS   = 0x22,  // output a word string
    TRAP_IN     = 0x23,    // get character from keyboard, echoed onto the terminal
    TRAP_PUTSP  = 0x24, // output a byte string
    TRAP_HALT   = 0x25   // halt the program
};

enum /* Memory Mapped registers */ {
    MR_KBSR     = 0xFE00,   // keyboard status
    MR_KBDR     = 0xFE02    // keyboard data
};

}

#endif // D814E86E_6F32_4D61_B970_E93757D187C4
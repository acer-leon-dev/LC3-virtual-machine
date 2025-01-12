#include "lc3.h"


namespace lc3
{

// ADD+: Addition
//
// Assembler Format:
//  ADD DR, SR1, SR2
//  ADD DR, SR1, imm5
//
// Encoding:
//  register:
//  15 12   11 9    8  6    5    4 3    2 0
//  0001    DR__    SR1_    0    00_    SR2
//  immediate value:
//  15 12   11 9    8  6    5    4  0
//  0001    DR__    SR1_    1    imm5
void LC3::m_op_ADD(lc3_size_t instruction) {
    // destination register (DR)
    register_t dest_reg = bit::get_arg(instruction, 11, 9); 
    // first operand (srce_reg1, source register)
    register_t srce_reg1 = bit::get_arg(instruction, 8, 6); 
    // whether we are in immediate mode
    bool immediate_bit = (instruction >> 5) & 0b1; 

    if (immediate_bit) {
        lc3_size_t imm_val = bit::sext(instruction & 0b11111, 5);
        m_registers.set(dest_reg, m_registers.get(srce_reg1) + imm_val);
    }
    else {
        register_t srce_reg2 = instruction & 0b111;
        m_registers.set(dest_reg, m_registers.get(srce_reg1) + m_registers.get(srce_reg2));
    }

    m_update_flags(dest_reg); // update condition flag
}

// AND+: Bitwise Logical AND
//
// Assembler Format:
//  AND DR, SR1, SR2
//  AND DR, sR1, imm5
//
// Encoding:
//  register:
//  15 12   11 9    8  6    5    4 3    2 0
//  0001    DR__    SR1_    0    00_    SR2
//  immediate value:
//  15 12   11 9    8  6    5    4  0
//  0001    DR__    SR1_    1    imm5
void LC3::m_op_AND(lc3_size_t instruction) {

    register_t dest_reg = bit::get_arg(instruction, 11, 9);
    register_t srce_reg1 = bit::get_arg(instruction, 8, 6);
    bool immediate_bit = (instruction >> 5) & 0b1;

    if (immediate_bit) {
        lc3_size_t imm_val = bit::sext(instruction && 0b11111, 5);
        m_registers.set(dest_reg, m_registers.get(srce_reg1) & imm_val); 
    } 
    else {
        lc3_size_t srce_reg2 = instruction & 0b111;
        m_registers.set(dest_reg, m_registers.get(srce_reg1) & m_registers.get(srce_reg2));
    }
}

// NOT+: Bitwise Complement (Bitwise Logical NOT)
//
// Assembler Format:
//  NOT SR, DR  
//
// Encoding:
//  15 12   11 9    8  6    5    4  0
//  1001    DR__    SR__    1    1111
void LC3::m_op_NOT(lc3_size_t instruction) {

    register_t dest_reg = bit::get_arg(instruction, 11, 9);
    lc3_size_t srce_reg = bit::get_arg(instruction, 8, 6);
    m_registers.set(dest_reg, ~m_registers.get(srce_reg));
    m_update_flags(dest_reg);
}

// BR: Conditional Branch
//
// Assembler Formats:
//  BRn   LABEL 
//  BRzp  LABEL
//  BRz   LABEL 
//  BRnp  LABEL
//  BRp   LABEL 
//  BRnz  LABEL
//  BR    LABEL 
//  BRnzp LABEL
//
// Encoding:
//  15 12   11   10   9    8       0
//  0000    n    z    p    PCoffset9
void LC3::m_op_BR(lc3_size_t instruction) {
    // True if [bit] is set
    bool n_bit = (instruction >> 11) & 0b1;
    bool z_bit = (instruction >> 10) & 0b1;
    bool p_bit = (instruction >> 9) & 0b1;
    // Gets the status of each condition flag
    bool N = m_registers.get(R_COND) == FL_NEG;
    bool Z = m_registers.get(R_COND) == FL_ZRO;
    bool P = m_registers.get(R_COND) == FL_POS;    
    if ((n_bit && N) || (z_bit && Z) || (p_bit && P)) {
        m_registers.inc(R_PC, bit::sext(bit::get_arg(instruction, 9, bit::label::end), 9));
    }
}

// JMP: Jump
//
// Assembler Formats:
//  JMP BaseR
//  RET
//
// Encoding:
//  JMP:
//  15 12    11 9    8   6    5    0
//  1100     000_    BaseR    000000
//
//  RET:
//  15 12    11 9    8 6    5    0
//  1100     000_    111    000000
void LC3::m_op_JMP(lc3_size_t instruction) {
    lc3_size_t base_reg = bit::get_arg(instruction, 8, 6); // baseR
    m_registers.set(R_PC, m_registers.get(base_reg)); // move IP to location
}

// Assembler Formats:
//  JSR LABEL
//  JSR BaseR
//
// Encoding:
//  JSR:
//  15 12   11   10       0
//  0100    1    PCoffset11
//  JSRR:
//  15 12   11   10 9    8   6    5    0    
//  0100    0    00__    BaseR    000000
void LC3::m_op_JSR(lc3_size_t instruction) {
    m_registers.set(R_R7, m_registers.get(R_PC));
    if (bit::get_arg(instruction, bit::label::end, 11)) {
        lc3_size_t pc_offset = bit::sext(bit::get_arg(instruction, 10, bit::label::end), 10);
        m_registers.inc(R_PC, pc_offset); // move PC by pc_offset
    }
    else {
        // set pc to address stored in base_reg
        lc3_size_t base_reg = bit::get_arg(instruction, 8, 6);
        m_registers.set(R_PC, base_reg);
    }
}

// Assembler Format:
//  LD DR, LABEL
// 
// Encoding:
//  15 12   11 9    8       0
//  0010    DR__    PCoffset9
void LC3::m_op_LD(lc3_size_t instruction) {
    // destination register (DR)
    register_t dest_reg = bit::get_arg(instruction, 11, 9);
    // PCoffset9
    lc3_size_t pc_offset = bit::sext(bit::get_arg(instruction, 8, bit::label::end), 8);
    m_registers.set(dest_reg, m_memory.read(m_registers.get(R_PC) + pc_offset));
    m_update_flags(dest_reg);

}

// Assembler Format:
//  LDI DR, LABEL
// 
// Encoding:
//  15 12   11 9    8       0
//  1010    DR__    PCoffset9
void LC3::m_op_LDI(lc3_size_t instruction) {
    // destination register (DR)
    register_t dest_reg = bit::get_arg(instruction, 11, 9);
    // PCoffset9
    lc3_size_t pc_offset = bit::sext(instruction & 0x111111111, 9);
    // add pc_offset to the current PC, look at that memory_storage location to get the final address
    lc3_size_t address = m_memory.read(m_registers.get(R_PC) + pc_offset);
    m_registers.set(dest_reg, m_memory.read(address));
    m_update_flags(dest_reg);
}

// Assembler Format:
//  LDR DR, BaseR, offset6
// 
// Encoding:
// 15 12   11 9    8   6    5     0
// 0110    DR__    BaseR    offset6
void LC3::m_op_LDR(lc3_size_t instruction) {
    register_t dest_reg = bit::get_arg(instruction, 11, 9);
    lc3_size_t base_reg = bit::get_arg(instruction, 8, 6);
    lc3_size_t offset = bit::sext(bit::get_arg(instruction, 6, bit::label::end), 6);
    m_registers.set(dest_reg, m_memory.read(m_registers.get(base_reg) + offset));
    m_update_flags(dest_reg);
}

// Assembler Format:
//  LEA DR, LABEL
//
// Encoding:
//  15 12   11 9    8       0
//  1110    DR__    PCoffset9
void LC3::m_op_LEA(lc3_size_t instruction) {
    register_t dest_reg = bit::get_arg(instruction, 11, 9);
    lc3_size_t pc_offset = bit::sext(bit::get_arg(instruction, 9, bit::label::end), 9);
    m_registers.set(dest_reg, m_registers.get(R_PC) + pc_offset);
    m_update_flags(dest_reg);
}

// Assembler Format:
//  ST SR, LABEL
//
// Encoding:
//  15 12   11 9    8       0
//  0011    SR__    PCoffset9
void LC3::m_op_ST(lc3_size_t instruction) {
    lc3_size_t srce_reg = bit::get_arg(instruction, 11, 9);
    lc3_size_t pc_offset = bit::sext(bit::get_arg(instruction, 9, bit::label::end), 9);
    m_memory.write(m_registers.get(R_PC) + pc_offset, m_registers.get(srce_reg));
}

// Assembler Format:
//  STI SR, LABEL  
//
// Encoding:
//  15 12   11 9    8       0
//  1011    SR__    PCoffset9
void LC3::m_op_STI(lc3_size_t instruction) {
    lc3_size_t srce_reg = bit::get_arg(instruction, 11, 9);
    lc3_size_t pc_offset = bit::sext(bit::get_arg(instruction, 9, bit::label::end), 9);
    m_memory.write(m_memory.read(m_registers.get(R_PC) + pc_offset), m_registers.get(srce_reg));
}

// Assembler Format:
//  STR SR, BaseR, offset6  
//
// Encoding
//  15 12   11 9    8   6    5     0
//  0111    SR__    BaseR    offset6
void LC3::m_op_STR(lc3_size_t instruction) {
    lc3_size_t srce_reg = bit::get_arg(instruction, 11, 9);
    lc3_size_t base_reg = bit::get_arg(instruction, 8, 6);
    lc3_size_t pc_offset = bit::sext(bit::get_arg(instruction, 6, bit::label::end), 6);
    m_memory.write(m_registers.get(base_reg) + pc_offset, m_registers.get(srce_reg));
}

// Assembler Format:
//  TRAP trapvector8
// Encoding
//  15 12   11 8    7       0
//  1111    0000    trapvect8
void LC3::m_op_TRAP(lc3_size_t instruction)
{ 
    m_registers.set(R_R7, m_registers.get(R_PC));
    switch (bit::get_arg(instruction, 8, bit::label::end)) {
        case TRAP_GETC:     m_trap_GETC();    break;
        case TRAP_OUT:      m_trap_OUT();     break;
        case TRAP_PUTS:     m_trap_PUTS();    break;
        case TRAP_IN:       m_trap_IN();      break;
        case TRAP_PUTSP:    m_trap_PUTSP();   break;
        case TRAP_HALT:     m_trap_HALT();    break;
        default: break;
    }
}   


// Encoding:
//  15 12   11         0
//  1000    000000000000
void LC3::m_op_RTI(lc3_size_t instruction) {
    std::println(stderr, "'RTI' ({:04b}) is not a valid opcode.", static_cast<int>(OP_RTI));
    std::println(stderr, "\tnote: 'RTI' ({:04b}) is not in use.", static_cast<int>(OP_RTI));
    std::abort();
}

// Unused Opcode
//
// Encoding:
//  15 12   11         0
//  1101    000000000000
void LC3::m_op_RES(lc3_size_t instruction) {
    std::println(stderr, "'reserved' ({:04b}) is not a valid opcode.", static_cast<int>(OP_RES));
    std::println(stderr, "\tnote: 'reserved' ({:04b}) is not in use.", static_cast<int>(OP_RES));
    std::abort();
}

void LC3::m_op_ERROR(lc3_size_t instruction) {
    std::println(stderr, "'unknown' ({:04b}) is not a valid opcode.", bit::get_arg(instruction, bit::label::end, 12));
    std::abort();
}

}
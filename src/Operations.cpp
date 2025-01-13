#include "LC3VM/LC3.h"

namespace lc3
{

// ADD+ : Addition
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
void LC3::m_op_ADD(lc3_size_t instruction)
{
    // destination register (DR)
    register_t dest_reg = bit::cut(instruction, 11, 9); 
    // first operand (source_reg1, source register)
    register_t source_reg1 = bit::cut(instruction, 8, 6); 
    // whether we are in immediate mode
    bool immediate_bit = bit::get_arg(instruction, 5); 

    if (immediate_bit)
    {
        lc3_size_t val = bit::sext(bit::cut(instruction, 4, 0), 5);
        m_reg.set(dest_reg, m_reg.get(source_reg1) + val);
    }
    else
    {
        register_t source_reg2 = instruction & 0b111;
        m_reg.set(dest_reg, m_reg.get(source_reg1) + m_reg.get(source_reg2));
    }

    m_reg.update_flags(); // update condition flag
}

// AND+ : Bitwise Logical AND
//
// Assembler Format:
//  AND DR, SR1, SR2
//  AND DR, SR1, imm5
//
// Encoding:
//  register:
//  15 12   11 9    8  6    5    4 3    2 0
//  0001    DR__    SR1_    0    00_    SR2
//  immediate value:
//  15 12   11 9    8  6    5    4  0
//  0001    DR__    SR1_    1    imm5
void LC3::m_op_AND(lc3_size_t instruction)
{

    register_t dest_reg = bit::cut(instruction, 11, 9);
    register_t source_reg1 = bit::cut(instruction, 8, 6);
    bool immediate_bit = bit::get_arg(instruction, 5);

    if (immediate_bit)
    {
        lc3_size_t val = bit::sext(bit::cut(instruction, 4, 0), 5);
        m_reg.set(dest_reg, m_reg.get(source_reg1) & val); 
    } 
    else
    {
        lc3_size_t source_reg2 = bit::cut(instruction, 3, 0);
        m_reg.set(dest_reg, m_reg.get(source_reg1) & m_reg.get(source_reg2));
    }

    m_reg.update_flags();
}

// NOT+ : Bitwise Complement (Bitwise Logical NOT)
//
// Assembler Format:
//  NOT SR, DR  
//
// Encoding:
//  15 12   11 9    8  6    5    4  0
//  1001    DR__    SR__    1    1111
void LC3::m_op_NOT(lc3_size_t instruction)
{
    register_t dest_reg = bit::cut(instruction, 11, 9);
    register_t source_reg = bit::cut(instruction, 8, 6);
    m_reg.set(dest_reg, ~m_reg.get(source_reg));
    m_reg.update_flags();
}

// BR : Conditional Branch
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
void LC3::m_op_BR(lc3_size_t instruction)
{
    // True if [bit] is set
    bool flags = bit::cut(instruction, 11, 9);    
    if (flags & m_reg.get(R_COND))
    {
        lc3_size_t pc_off = bit::sext(bit::cut(instruction, 8, 0), 9);
        m_reg.inc(R_PC, pc_off);
    }
}

// JMP : Jump
// RET : Return from Subroutine
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
void LC3::m_op_JMP(lc3_size_t instruction)
{
    register_t base_reg = bit::cut(instruction, 8, 6); // baseR
    m_reg.set(R_PC, m_reg.get(base_reg)); // move IP to location
}

// JSR : Jump to Subroutine
// JSRR
//
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
void LC3::m_op_JSR(lc3_size_t instruction)
{
    m_reg.set(R_R7, m_reg.get(R_PC));
    if (bit::get_arg(instruction, 11))
    {
        lc3_size_t pc_off = bit::sext(bit::cut(instruction, 10, 0), 11);
        m_reg.inc(R_PC, pc_off); // move PC by pc_off
    }
    else
    {
        // set pc to address stored in base_reg
        register_t base_reg = bit::cut(instruction, 8, 6);
        m_reg.set(R_PC, base_reg);
    }
}

// LD+ : Load
//
// Assembler Format:
//  LD DR, LABEL
// 
// Encoding:
//  15 12   11 9    8       0
//  0010    DR__    PCoffset9
void LC3::m_op_LD(lc3_size_t instruction)
{
    register_t dest_reg = bit::cut(instruction, 11, 9);
    lc3_size_t pc_off = bit::sext(bit::cut(instruction, 8, 0), 9);
    m_reg.set(dest_reg, m_mem.read(m_reg.get(R_PC) + pc_off));
    m_reg.update_flags();
}

// LDI+ : Load Indirect
//
// Assembler Format:
//  LDI DR, LABEL
// 
// Encoding:
//  15 12   11 9    8       0
//  1010    DR__    PCoffset9
void LC3::m_op_LDI(lc3_size_t instruction)
{
    register_t dest_reg = bit::cut(instruction, 11, 9);
    lc3_size_t pc_off = bit::sext(bit::cut(instruction, 8, 0), 9);
    // add pc_off to the current PC, look at that memory_storage location to get the final address
    lc3_size_t address = m_mem.read(m_reg.get(R_PC) + pc_off);
    m_reg.set(dest_reg, m_mem.read(address));
    m_reg.update_flags();
}

// LDR+ : Load Base+offset
//
// Assembler Format:
//  LDR DR, BaseR, offset6
// 
// Encoding:
// 15 12   11 9    8   6    5     0
// 0110    DR__    BaseR    offset6
void LC3::m_op_LDR(lc3_size_t instruction)
{
    register_t dest_reg = bit::cut(instruction, 11, 9);
    register_t base_reg = bit::cut(instruction, 8, 6);
    lc3_size_t off = bit::sext(bit::cut(instruction, 5, 0), 6);
    m_reg.set(dest_reg, m_mem.read(m_reg.get(base_reg) + off));
    m_reg.update_flags();
}

// LEA+ : Load Effective Address
//
// Assembler Format:
//  LEA DR, LABEL
//
// Encoding:
//  15 12   11 9    8       0
//  1110    DR__    PCoffset9
void LC3::m_op_LEA(lc3_size_t instruction)
{
    register_t dest_reg = bit::cut(instruction, 11, 9);
    lc3_size_t pc_off = bit::sext(bit::cut(instruction, 8, 0), 9);
    m_reg.set(dest_reg, m_reg.get(R_PC) + pc_off);
    m_reg.update_flags(dest_reg);
}

// ST : Store
//
// Assembler Format:
//  ST SR, LABEL
//
// Encoding:
//  15 12   11 9    8       0
//  0011    SR__    PCoffset9
void LC3::m_op_ST(lc3_size_t instruction)
{
    register_t source_reg = bit::cut(instruction, 11, 9);
    lc3_size_t pc_off = bit::sext(bit::cut(instruction, 8, 0), 9);
    m_mem.write(m_reg.get(R_PC) + pc_off, m_reg.get(source_reg));
}

// STI : Store Indirect
//
// Assembler Format:
//  STI SR, LABEL  
//
// Encoding:
//  15 12   11 9    8       0
//  1011    SR__    PCoffset9
void LC3::m_op_STI(lc3_size_t instruction)
{
    register_t source_reg = bit::cut(instruction, 11, 9);
    lc3_size_t pc_off = bit::sext(bit::cut(instruction, 8, 0), 9);
    m_mem.write(m_mem.read(m_reg.get(R_PC) + pc_off), m_reg.get(source_reg));
}

// STR : Store Base+offset
//
// Assembler Format:
//  STR SR, BaseR, offset6  
//
// Encoding
//  15 12   11 9    8   6    5     0
//  0111    SR__    BaseR    offset6
void LC3::m_op_STR(lc3_size_t instruction)
{
    register_t source_reg = bit::cut(instruction, 11, 9);
    register_t base_reg = bit::cut(instruction, 8, 6);
    lc3_size_t pc_off = bit::sext(bit::cut(instruction, 5, 0), 6);
    m_mem.write(m_reg.get(base_reg) + pc_off, m_reg.get(source_reg));
}

// TRAP : System Call
//
// Assembler Format:
//  TRAP trapvector8
// Encoding
//  15 12   11 8    7       0
//  1111    0000    trapvect8
void LC3::m_op_TRAP(lc3_size_t instruction)
{ 
    m_reg.set(R_R7, m_reg.get(R_PC));
    switch (bit::cut(instruction, 7, 0))
    {
        case TRAP_GETC:     m_trap_GETC();    break;
        case TRAP_OUT:      m_trap_OUT();     break;
        case TRAP_PUTS:     m_trap_PUTS();    break;
        case TRAP_IN:       m_trap_IN();      break;
        case TRAP_PUTSP:    m_trap_PUTSP();   break;
        case TRAP_HALT:     m_trap_HALT();    break;
        default: break;
    }
}

// RTI : Return from Interrupt
//
// Encoding:
//  15 12   11         0
//  1000    000000000000
void LC3::m_op_RTI(lc3_size_t instruction)
{
    std::println(stderr, "'RTI' ({:04b}) is not a valid opcode.", static_cast<int>(OP_RTI));
    std::println(stderr, "\tnote: 'RTI' ({:04b}) is not in use. Do not use this operation in source code.", static_cast<int>(OP_RTI));
    std::abort();
}

// Reserved/Unused Opcode
//
// Encoding:
//  15 12   11         0
//  1101    000000000000
void LC3::m_op_RES(lc3_size_t instruction)
{
    std::println(stderr, "'reserved' ({:04b}) is not a valid opcode.", static_cast<int>(OP_RES));
    std::println(stderr, "\tnote: 'reserved' ({:04b}) is not in use. Do not use this operation in source code.", static_cast<int>(OP_RES));
    std::abort();
}

// Invalid opcode 
void LC3::m_op_ERROR(lc3_size_t instruction)
{
    std::println(stderr, "'unknown' ({:04b}) is not a valid opcode.", bit::cut(instruction, bit::label::msb, 12));
    std::abort();
}

}
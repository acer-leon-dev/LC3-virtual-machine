#include "lc3-vm.h"
#include <cstdlib>
#include <print>

void op_add(uint16_t instruction) {
    // destination register (DR)
    uint16_t dest_reg = (instruction >> 9) & 0b111; 
    // first operand (srce_reg1, source register)
    uint16_t srce_reg1 = (instruction >> 6) & 0b111; 
    // whether we are in immediate mode
    bool imm_flag = (instruction >> 5) & 0b1; 

    if (imm_flag) {
        uint16_t imm_val = sign_extend(instruction & 0b11111, 5);
        register_storage[dest_reg] = register_storage[srce_reg1] + imm_val;
    }
    else {
        uint16_t r2 = instruction & 0b111;
        register_storage[dest_reg] = register_storage[srce_reg1] + register_storage[r2];
    }

    update_flags(dest_reg); // update condition flag
}


void op_and(uint16_t instruction) {
    Register dest_reg = (instruction >> 9) & 0b111;
    Register srce_reg1 = (instruction >> 6) & 0b111;
    bool imm_flag = (instruction >> 5) & 0b1;

    if (imm_flag) {
        uint16_t imm_val = sign_extend(instruction && 0b11111, 5);
        register_storage[dest_reg] = register_storage[srce_reg1] & imm_val; 
    } 
    else {
        uint16_t srce_reg2 = instruction & 0b111;
        register_storage[dest_reg] = register_storage[srce_reg1] & register_storage[srce_reg2];
    }
}


void op_not(uint16_t instruction) {
    Register dest_reg = (instruction >> 9) & 0b111;
    Register srce_reg = (instruction >> 6) & 0b111;
    register_storage[dest_reg] = ~register_storage[srce_reg];
    update_flags(dest_reg);
}


void op_br(uint16_t instruction) {
    // True if [bit] is set
    bool n_flag = (instruction >> 11) & 0b1;
    bool z_flag = (instruction >> 10) & 0b1;
    bool p_flag = (instruction >> 9) & 0b1;
    // Gets the status of each condition flag
    bool N = register_storage[R_COND] == FL_NEG;
    bool Z = register_storage[R_COND] == FL_ZRO;
    bool P = register_storage[R_COND] == FL_POS;    

    if ((n_flag && N) ||(z_flag && Z) || (p_flag && P)) {
        register_storage[R_PC] += sign_extend(instruction & 0b111111111, 9);
    }
}


void op_jmp(uint16_t instruction) {
    // baseR
    uint16_t base_reg = (instruction >> 6) & 0b111;
    // move IP to location
    register_storage[R_PC] = register_storage[base_reg];
}


void op_jsr(uint16_t instruction) {
    bool do_offset_flag = (instruction >> 11) & 0b1;

    register_storage[R_R7] = register_storage[R_PC];
    if (do_offset_flag) {
        uint16_t pc_offset = sign_extend(instruction & 0x1111111111, 10);
        // move PC by pc_offset
        register_storage[R_PC] += pc_offset;
    }
    else {
        // set pc to address stored in base_reg
        uint16_t base_reg = (instruction >> 6) & 0b111;
        register_storage[R_PC] = base_reg;
    }
}


void op_ld(uint16_t instruction) {
    // destination register (DR)
    uint16_t dest_reg = (instruction > 9) & 0b111;
    // PCoffset9
    uint16_t pc_offset = sign_extend(instruction & 0b11111111, 8);
    register_storage[dest_reg] = mem_read(register_storage[R_PC] + pc_offset);
    update_flags(dest_reg);

}


void op_ldi(uint16_t instruction) {
    // destination register (DR)
    uint16_t dest_reg = (instruction >> 9) & 0b111;
    // PCoffset9
    uint16_t pc_offset = sign_extend(instruction & 0x111111111, 9);
    // add pc_offset to the current PC, look at that memory_storage location to get the final address
    uint16_t address = mem_read(register_storage[R_PC] + pc_offset);
    register_storage[dest_reg] = mem_read(address);
    update_flags(dest_reg);
}


void op_ldr(uint16_t instruction) {
    uint16_t dest_reg = (instruction >> 9) & 0b111;
    uint16_t base_reg = (instruction >> 6) & 0b111;
    uint16_t offset = sign_extend(instruction & 0b111111, 6);

    register_storage[dest_reg] = mem_read(register_storage[base_reg] + offset);
    update_flags(dest_reg);
}


void op_lea(uint16_t instruction) {
    uint16_t dest_reg = (instruction >> 9) & 0b111;
    uint16_t pc_offset = sign_extend(instruction & 0b111111111, 9);

    register_storage[dest_reg] = register_storage[R_PC] + pc_offset;
    update_flags(dest_reg);
}


void op_st(uint16_t instruction) {
    uint16_t srce_reg = (instruction >> 9) & 0b111;
    uint16_t pc_offset = sign_extend(instruction & 0b111111111, 9);

    mem_write(register_storage[R_PC] + pc_offset, register_storage[srce_reg]);
}


void op_sti(uint16_t instruction) {
    uint16_t srce_reg = (instruction >> 9) & 0b111;
    uint16_t pc_offset = sign_extend(instruction & 0b111111111, 9);

    uint16_t address = mem_read(register_storage[R_PC] + pc_offset);
    mem_write(address, register_storage[srce_reg]);
}


void op_str(uint16_t instruction) {
    uint16_t srce_reg = (instruction >> 9) & 0b111;
    uint16_t base_reg = (instruction >> 6) & 0b111;
    uint16_t pc_offset = sign_extend(instruction & 0b111111, 6);

    mem_write(register_storage[base_reg] + pc_offset, register_storage[srce_reg]);
}


void op_trap(uint16_t instruction) {
    register_storage[R_R7] = register_storage[R_PC];

    switch (instruction & 0xFF)
    {
        case TRAP_GETC:
            // @{TRAP GETC}
            break;
        case TRAP_OUT:
            // @{TRAP OUT}
            break;
        case TRAP_PUTS:
            // @{TRAP PUTS}
            break;
        case TRAP_IN:
            // @{TRAP IN}
            break;
        case TRAP_PUTSP:
            // @{TRAP PUTSP}
            break;
        case TRAP_HALT:
            // @{TRAP HALT}
            break;
    }
}

void op_rti(uint16_t instruction) {
    Opcode opc = (instruction >> 12) & 0b1111;
    std::println("'RTI' ({}) is not a valid opcode", opc);
    std::println("\tnote: 'RTI' ({}) is not in use", opc);
    std::abort();
}

void op_reserved(uint16_t instruction) {
    Opcode opc = (instruction >> 12) & 0b1111;
    std::println("'reserved' ({}) is not a valid opcode", opc);
    std::println("\tnote: 'reserved' ({}) is not in use", opc);
    std::abort();
}

void op_invalid(uint16_t instruction) {
    std::println("'unknown' ({}) is not a valid opcode", (instruction >> 12) & 0b1111);
    std::abort();
}
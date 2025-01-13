#include "LC3VM/LC3.h"
#include <csignal>

namespace lc3 
{

namespace
{

std::string opcode_to_str(uint8_t opcode)
{
    switch (opcode) 
    {
        case OP_ADD:  return "ADD";
        case OP_AND:  return "AND";
        case OP_NOT:  return "NOT";
        case OP_BR:   return "BR";
        case OP_JMP:  return "JMP";
        case OP_JSR:  return "JSR";
        case OP_LD:   return "LD";
        case OP_LDI:  return "LDI";
        case OP_LDR:  return "LDR";
        case OP_LEA:  return "LEA";
        case OP_ST:   return "ST";
        case OP_STI:  return "STI";
        case OP_STR:  return "STR";
        case OP_TRAP: return "TRAP";
        case OP_RES:  return "reserved";
        case OP_RTI:  return "RTI";
        default: return "";
    }
}

}


LC3::LC3()
    : m_running(false)
{
    // Set condition register to ZERO by default
    m_reg.set(R_COND, FL_ZRO);
    // Set PC to x3000 by default
    m_reg.set(R_PC, PC_START);
}

LC3::~LC3() 
{
}

void LC3::run() 
{
    std::signal(SIGINT, s_handle_interrupt);
    s_disable_input_buffering();

    // Test 1 // // // // // // // // // // // // // // //
    // Create program
    // std::vector<lc3_size_t> program1;
    // for (int i = 0; i < 36; i++) 
    // {
    //     program1.push_back(static_cast<lc3_size_t>((OP_TRAP << 12) + m_trap_GETC));
    //     program1.push_back(static_cast<lc3_size_t>((OP_TRAP << 12) + m_trap_OUT));
    // }
    // program1.push_back(static_cast<lc3_size_t>((OP_TRAP << 12) + m_trap_HALT));

    // // Copy program into (vm) memory
    // std::copy(program1.begin(), program1.end(),
    //     m_mem.internal.begin() + PC_START);
    // // // // // // // // // // // // // // // // // //

    // std::println("Running...");
    m_running = true;
    while (m_running)
    {
        // Read the next instruction and retrieve its opcode
        lc3_size_t addr = m_reg.get(R_PC);
        lc3_size_t instr = m_mem.read(addr);
        lc3_size_t op = bit::cut(instr, bit::label::msb, 12);
        m_reg.inc(R_PC, 1);
        
        // Test
        std::print("R_PC = {}, ", m_reg.get(R_PC) - 1);
        std::print("memory[R_PC] = {:016b}, ", m_mem.read(m_reg.get(R_PC) - 1));
        std::print("opcode = {}/{}", op, opcode_to_str(op));
        std::println();
        std::cout.flush();

        
        // Match opcode to its respective function
        switch (op) 
        {
            case OP_ADD:  m_op_ADD(instr); break;
            case OP_AND:  m_op_AND(instr); break;
            case OP_NOT:  m_op_NOT(instr); break;
            case OP_BR:   m_op_BR(instr); break;
            case OP_JMP:  m_op_JMP(instr); break;
            case OP_JSR:  m_op_JSR(instr); break;
            case OP_LD:   m_op_LD(instr); break;
            case OP_LDI:  m_op_LDI(instr); break;
            case OP_LDR:  m_op_LDR(instr); break;
            case OP_LEA:  m_op_LEA(instr); break;
            case OP_ST:   m_op_ST(instr); break;
            case OP_STI:  m_op_STI(instr); break;
            case OP_STR:  m_op_STR(instr); break;
            case OP_TRAP: m_op_TRAP(instr); break;
            case OP_RES:  /* m_op_RES(instr); */ break;
            case OP_RTI:  /* m_op_RTI(instr); */ break;
            default:      m_op_ERROR(instr); break;
        }
    }

    s_restore_input_buffering();
}

bool LC3::read_image(const std::string& path) 
{
    std::ifstream fobj(path, std::ios::binary);
    if (!fobj) 
    {
        return false;
    }

    // Determine the origin (beginning location) of the program
    lc3_size_t origin;  
    fobj.read(reinterpret_cast<char*>(&origin), sizeof(lc3_size_t));
    origin = std::byteswap(origin); // switch from little to big endian
    m_reg.set(R_PC, origin);
    
    // Read rest of the the program
    std::vector<lc3_size_t> buffer;
    lc3_size_t info;
    while (fobj.read(reinterpret_cast<char*>(&info), sizeof(lc3_size_t))) 
    {
        buffer.push_back(std::move(std::byteswap(info)));
    }

    // Copy program to memory
    std::copy(buffer.begin(), buffer.end(), m_mem.m_internal.begin() + origin);

    // Test
    // // Print program output
    // std::println("Loaded '{}':", path);
    // auto begin = m_mem.m_internal.begin() + origin;
    // auto end = begin + buffer.size();
    // for (auto it = begin; it != end; it++)
    // {
    //     std::println("\t{0:016b} {0:#06x}", *it);
    // }

    fobj.close();
    return true;
}

} // lc3
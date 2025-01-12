#include "lc3.h"
#include <csignal>

namespace lc3 
{

LC3::LC3()
    : running(false)
{
    // Set condition register to zero by default
    m_registers.set(R_COND, FL_ZRO);
    // Set PC to x3000 by default
    m_registers.set(R_PC, PC_START);
}

LC3::~LC3() 
{
    m_restore_input_buffering();
}

void LC3::run() 
{
    std::signal(SIGINT, m_handle_interrupt);
    m_disable_input_buffering();

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
    //     m_memory.internal.begin() + PC_START);
    // // // // // // // // // // // // // // // // // //

    std::println("Running...");
    running = true;
    while (running) 
    {
        // Read the next instruction and retrieve its opcode
        lc3_size_t instr = m_memory.read(m_registers.get(R_PC));
        m_registers.inc(R_PC, 1);
        lc3_size_t op = bit::get_arg(instr, bit::label::end, 12);
        
        // Test 
        // std::print("R_PC = {}, ", registers.at(R_PC));
        // std::print("Memory[R_PC] = {:016b}, ", m_memory.read(registers.at(R_PC)));
        // std::println("Opcode = {}", op);

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
            case OP_RES:  m_op_RES(instr); break;
            case OP_RTI:  m_op_RTI(instr); break;
            default:      m_op_ERROR(instr); break;
        }
    }
}

bool LC3::read_image(const std::string& path) 
{
    std::ifstream fobj(path, std::ios::binary);
    if (!fobj) 
    {
        return false;
    }

    //Determine the origin (beginning location) of the program
    lc3_size_t origin;  
    fobj.read(reinterpret_cast<char*>(&origin), sizeof(lc3_size_t));
    origin = std::byteswap(origin); // switch from little to big endian
    
    // Read rest of the the program
    std::vector<lc3_size_t> buffer;
    lc3_size_t info;
    while (fobj.read(reinterpret_cast<char*>(&info), sizeof(lc3_size_t))) 
    {
        buffer.push_back(std::move(std::byteswap(info)));
    }

    // Copy program to memory
    std::copy(buffer.begin(), buffer.end(), m_memory.internal.begin() + origin);

    // Test
    // // Print program output
    // std::println("Loaded '{}':", path);
    // for (auto line : buffer)
    //     std::println("\t{0:016b}, {0:#06x}", line);
    fobj.close();
    return true;
}

void LC3::m_update_flags(register_t reg) 
{
    if (m_registers.get(reg) == 0) 
    {
        m_registers.set(R_COND, FL_ZRO);
    }
    // a 1 in the left-most bit indicates negative
    else if (m_registers.get(reg) >> 15)
    
    {
        m_registers.set(R_COND, FL_NEG);
    }
    else
    
    {
        m_registers.set(R_COND, FL_POS);
    }
}

} // lc3
#ifndef EC04C2DD_3AC3_4AF2_BBB8_052F21CA2D14
#define EC04C2DD_3AC3_4AF2_BBB8_052F21CA2D14

#include "LC3VM/lc3_types.h"
#include "LC3VM/lc3_Memory.h"
#include "LC3VM/lc3_RegisterFile.h"

namespace lc3
{

class LC3
{
public:
    ///
    /// Constructor
    LC3();
    
    ///
    /// Destructor
    ~LC3();

    ///
    /// @brief Starts up the VM
    void run();

    ///
    /// @brief 
    /// @param path 
    /// @return True if the read was success; false if failed
    bool read_image(const std::string& path);
private:
    friend class Memory;
private:
    RegisterFile m_registers; // RegisterFile Storage
    Memory m_memory; // 65536 locations
    bool m_running;
private:
    static void s_disable_input_buffering();
    static void s_restore_input_buffering();
    static bool s_check_key();
    static void s_handle_interrupt(int signal);

    // Traps
    void m_trap_GETC();
    void m_trap_OUT();
    void m_trap_PUTS();
    void m_trap_IN();
    void m_trap_PUTSP();
    void m_trap_HALT();

    // Operations

    ///
    /// ADD+ : Addition
    void m_op_ADD(lc3_size_t instruction);

    ///
    /// AND+ : Bitwise Logical AND
    void m_op_AND(lc3_size_t instruction);

    ///
    /// NOT+ : Bitwise Complement (Bitwise Logical NOT)
    void m_op_NOT(lc3_size_t instruction);

    ///
    /// BR : Conditional Branch
    void m_op_BR(lc3_size_t instruction);

    /// JMP : Jump
    /// RET : Return from Subroutine
    void m_op_JMP(lc3_size_t instruction);

    /// JSR : Jump to Subroutine
    /// JSRR
    void m_op_JSR(lc3_size_t instruction);

    /// 
    /// LD+ : Load
    void m_op_LD(lc3_size_t instruction);

    /// 
    /// LDI+ : Load Indirect
    void m_op_LDI(lc3_size_t instruction);

    /// 
    /// LDR+ : Load Base+offset
    void m_op_LDR(lc3_size_t instruction);

    /// 
    /// LEA+ : Load Effective Address
    void m_op_LEA(lc3_size_t instruction);

    ///
    /// ST : Store
    void m_op_ST(lc3_size_t instruction);

    ///
    /// STI : Store Indirect
    void m_op_STI(lc3_size_t instruction);

    ///
    /// STR : Store Base+offset
    void m_op_STR(lc3_size_t instruction);

    ///
    /// TRAP : System Call
    void m_op_TRAP(lc3_size_t instruction);

    ///
    /// RTI : Return from Interrupt
    void m_op_RTI(lc3_size_t instruction);

    ///
    /// Reserved
    void m_op_RES(lc3_size_t instruction);

    ///
    /// Invalid opcode 
    void m_op_ERROR(lc3_size_t instruction);

}; // LC3
    
} // lc3

#endif /* EC04C2DD_3AC3_4AF2_BBB8_052F21CA2D14 */

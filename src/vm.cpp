#include "lc3-vm.h"
#include <cstdint>
#include <cstdlib>
#include <print>


int main(int argc, char** argv)
{
    if (argc < 2) {
        /* show usage string */
        std::println("lc3 [image-file1] ...");
        exit(2);
    }

    for (int i = 1; i < argc; i++) {
        if (!read_image(argv[i])) {
            std::println("failed to load image: {}", argv[i]);
            std::exit(1);
        }
    }

    setup_vm();

    int running = 1;
    while (running)
    {
        /* FETCH */
        uint16_t instr = mem_read(register_storage[R_PC]++);
        uint16_t op = instr >> 12;

        switch (op) {
            case OP_ADD:    { op_add(instr); break;     }
            case OP_AND:    { op_and(instr); break;     }
            case OP_NOT:    { op_not(instr); break;     }
            case OP_BR:     { op_br(instr); break;      }
            case OP_JMP:    { op_jmp(instr); break;     }
            case OP_JSR:    { op_jsr(instr); break;     }
            case OP_LD:     { op_ld(instr); break;      }
            case OP_LDI:    { op_ldi(instr); break;     }
            case OP_LDR:    { op_ldr(instr); break;     }
            case OP_LEA:    { op_lea(instr); break;     }
            case OP_ST:     { op_st(instr); break;      }
            case OP_STI:    { op_sti(instr); break;     }
            case OP_STR:    { op_str(instr); break;     }
            case OP_TRAP:   { op_trap(instr); break;    }
            case OP_RES:    { op_reserved(instr); break;    }
            case OP_RTI:    { op_rti(instr); break;     }
            default:        { op_invalid(instr); break; }
        }
    }
    
    // @{Shutdown}
}
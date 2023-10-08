#include "globals.h"
#include "structs_libraries_and_macros.h"
#include "opcode_flag_helper_functions.h"

// assume byte is of the form 0b00000xxx
uint16_t address_decoder_mode00(Survivor* survivor, uint8_t byte, uint16_t pos) {
    uint16_t addr;
    switch (byte) {
        case 0b000: {
            addr = survivor->BX + survivor->SI;
            return addr;
        }
        case 0b001: {
            addr = survivor->BX + survivor->DI;
            return addr;
        }
        case 0b010: {
            addr = survivor->BP + survivor->SI;
            return addr;
        }
        case 0b011: {
            addr = survivor->BP + survivor->DI;
            return addr;
        }
        case 0b100: {
            addr = survivor->SI;
            return addr;
        }
        case 0b101: {
            addr = survivor->DI;
            return addr;
        }
        case 0b110: {
            addr = memory[0].values[pos + 1] + memory[0].values[pos + 2] * 0x100;
            return addr;
        }
        case 0b111: {
            addr = survivor->BX;
            return addr;
        }
    }
    exit_angrily
}

uint16_t address_decoder_mode01(Survivor* survivor, uint8_t byte, uint16_t pos) {
    uint16_t addr;
    switch (byte) {
        case 0b000: {
            addr = survivor->BX + survivor->SI + memory[0].values[pos + 1];
            return addr;
        }
        case 0b001: {
            addr = survivor->BX + survivor->DI + memory[0].values[pos + 1];
            return addr;
        }
        case 0b010: {
            addr = survivor->BP + survivor->SI + memory[0].values[pos + 1];
            return addr;
        }
        case 0b011: {
            addr = survivor->BP + survivor->DI + memory[0].values[pos + 1];
            return addr;
        }
        case 0b100: {
            addr = survivor->SI + memory[0].values[pos + 1];
            return addr;
        }
        case 0b101: {
            addr = survivor->DI + memory[0].values[pos + 1];
            return addr;
        }
        case 0b110: {
            addr = survivor->BP + memory[0].values[pos + 1];
            return addr;
        }
        case 0b111: {
            addr = survivor->BX + memory[0].values[pos + 1];
            return addr;
        }
    }
    exit_angrily
}


uint16_t address_decoder_mode10(Survivor* survivor, uint8_t byte, uint16_t pos) {
    uint16_t addr;
    switch (byte) {
        case 0b000:
            {
            addr = survivor->BX + survivor->SI + memory[0].values[pos + 1] + memory[0].values[pos + 2] * 0x100;
            return addr;
            }
        case 0b001:
            {
            addr = survivor->BX + survivor->DI + memory[0].values[pos+1] + memory[0].values[pos+2]*0x100;
            return addr;
            }
        case 0b010:
            {
            addr = survivor->BP + survivor->SI + memory[0].values[pos+1] + memory[0].values[pos+2]*0x100;
            return addr;
            }
        case 0b011:
            {
            addr = survivor->BP + survivor->DI + memory[0].values[pos+1] + memory[0].values[pos+2]*0x100;
            return addr;
            }
        case 0b100:
            {
            addr = survivor->SI + memory[0].values[pos+1] + memory[0].values[pos+2]*0x100;
            return addr;
            }
        case 0b101:
            {
            addr = survivor->DI + memory[0].values[pos+1] + memory[0].values[pos+2]*0x100;
            return addr;
            }
        case 0b110:
            {
            addr = survivor->BP + memory[0].values[pos+1] + memory[0].values[pos+2]*0x100;
            return addr;
            }
        case 0b111:
            {
            addr = survivor->BX + memory[0].values[pos+1] + memory[0].values[pos+2]*0x100;
            return addr;
            }
    }
    exit_angrily
}


// TODO: determine if little endian assumption is right
// TODO: find way of implementing without switch case?
uint8_t* reg8_decoder(Survivor* survivor, uint8_t byte) {
    uint8_t *reg;
    switch (byte) {
        case 0b000: {
            reg = (uint8_t * ) & (survivor->AX);
            return reg;
        }
        case 0b001: {
            reg = (uint8_t * ) & (survivor->CX);
            return reg;
        }
        case 0b010: {
            reg = (uint8_t * ) & (survivor->DX);
            return reg;
        }
        case 0b011: {
            reg = (uint8_t * ) & (survivor->BX);
            return reg;
        }
        case 0b100: {
            reg = (uint8_t * ) & (survivor->AX) + 1;
            return reg;
        }
        case 0b101: {
            reg = (uint8_t * ) & (survivor->CX) + 1;
            return reg;
        }
        case 0b110: {
            reg = (uint8_t * ) & (survivor->DX) + 1;
            return reg;
        }
        case 0b111: {
            reg = (uint8_t * ) & (survivor->BX) + 1;
            return reg;
        }
    }
    exit_angrily
}

uint16_t* reg16_decoder(Survivor* survivor, uint8_t byte) {
    uint16_t* reg;
    switch (byte) {
        case 0b000: {
            reg = &(survivor->AX);
            return reg;
        }
        case 0b001: {
            reg = &(survivor->CX);
            return reg;
        }
        case 0b010: {
            reg = &(survivor->DX);
            return reg;
        }
        case 0b011: {
            reg = &(survivor->BX);
            return reg;
        }
        case 0b100: {
            reg = &(survivor->SP);
            return reg;
        }
        case 0b101: {
            reg = &(survivor->BP);
            return reg;
        }
        case 0b110: {
            reg = &(survivor->SI);
            return reg;
        }
        case 0b111: {
            reg = &(survivor->DI);
            return reg;
        }
    }
    exit_angrily
}


// General function to add two addresses while taking care of Flags
void general_add(Survivor* survivor, bool is_16_bit, uint8_t* significant_from, uint8_t* insignificant_from,
                 uint8_t* significant_to, uint8_t* insignificant_to)
{
    if (is_16_bit) {

    }
    else {
        int8_t result = *significant_to + *significant_from;
        survivor->Flags = flags_8_bit_add(*significant_to, *significant_from);
        *significant_to = result;
    }
}
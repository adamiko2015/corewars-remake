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
        survivor->Flags = flags_8_bit_add(*significant_to, *significant_from); // TODO: adam needs to make sure there is no funny business
        *significant_to = result;
    }
}

// might be a difference between our implementation and official implementation here.
// in the official implementation there is an exception when we push or pop from the end of the stack,
// here we just loop to the beginning of the segment.
bool general_push(Survivor* survivor, uint16_t shared_memory, uint16_t* reg)
{
    debug_print_statement
    survivor->SP -= 2;
    uint16_t destination = survivor->SP;

    uint16_t segment = ((destination+0x10*survivor->SS) & 0xF0000) >> 16; // stores the actual segment that will be written to, making sure it is legal
    if (segment != 0 && segment != survivor->stack_id && segment != shared_memory) {return false;}

    ((char*)memory)[(uint32_t) destination + 0x10*survivor->SS] = (*reg & 0xFF00) >> 8; // set first byte

    destination--;
    ((char*)memory)[(uint32_t) destination + 0x10*survivor->SS] += *reg & 0xFF; // set second byte

    survivor->IP += 1;

    return true;

}

bool general_pop(Survivor* survivor, uint16_t shared_memory, uint16_t* reg)
{
    debug_print_statement
    uint16_t address = survivor->SP;

    uint16_t segment = ((address+0x10*survivor->SS) & 0xF0000) >> 16;
    if (segment != 0 && segment != survivor->stack_id && segment != shared_memory) {return false;}

    *reg = ((char*)memory)[(uint32_t) address + 0x10*survivor->SS] << 8;

    address--;
    *reg |= ((char*)memory)[(uint32_t) address + 0x10*survivor->SS];

    survivor->SP += 2;

    survivor->IP += 1;

    return true;

}
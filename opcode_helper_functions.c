#include "globals.h"
#include "structs_libraries_and_macros.h"
#include "opcode_flag_helper_functions.h"
#include "opcode_helper_functions.h"

// assume byte is of the form 0b00000xxx
uint16_t address_decoder_mode00(Survivor survivor[static 1], uint8_t byte, uint16_t pos, uint8_t ip_progress[static 1]) {
    uint16_t addr;
    switch (byte) {
        case 0b000: {
            addr = sregs.BX + sregs.SI;
            return addr;
        }
        case 0b001: {
            addr = sregs.BX + sregs.DI;
            return addr;
        }
        case 0b010: {
            addr = sregs.BP + sregs.SI;
            return addr;
        }
        case 0b011: {
            addr = sregs.BP + sregs.DI;
            return addr;
        }
        case 0b100: {
            addr = sregs.SI;
            return addr;
        }
        case 0b101: {
            addr = sregs.DI;
            return addr;
        }
        case 0b110: {
            addr = memory[0].values[pos] + memory[0].values[pos + 1] * 0x100;
            *ip_progress += 2;
            return addr;
        }
        case 0b111: {
            addr = sregs.BX;
            return addr;
        }
    }
    exit_angrily
}

uint16_t address_decoder_mode01(Survivor survivor[static 1], uint8_t byte, uint16_t pos) {
    uint16_t addr;
    switch (byte) {
        case 0b000: {
            addr = sregs.BX + sregs.SI + memory[0].values[pos];
            return addr;
        }
        case 0b001: {
            addr = sregs.BX + sregs.DI + memory[0].values[pos];
            return addr;
        }
        case 0b010: {
            addr = sregs.BP + sregs.SI + memory[0].values[pos];
            return addr;
        }
        case 0b011: {
            addr = sregs.BP + sregs.DI + memory[0].values[pos];
            return addr;
        }
        case 0b100: {
            addr = sregs.SI + memory[0].values[pos];
            return addr;
        }
        case 0b101: {
            addr = sregs.DI + memory[0].values[pos];
            return addr;
        }
        case 0b110: {
            addr = sregs.BP + memory[0].values[pos];
            return addr;
        }
        case 0b111: {
            addr = sregs.BX + memory[0].values[pos];
            return addr;
        }
    }
    exit_angrily
}


uint16_t address_decoder_mode10(Survivor survivor[static 1], uint8_t byte, uint16_t pos) {
    uint16_t addr;
    switch (byte) {
        case 0b000:
            {
            addr = sregs.BX + sregs.SI + memory[0].values[pos] + memory[0].values[pos + 1] * 0x100;
            return addr;
            }
        case 0b001:
            {
            addr = sregs.BX + sregs.DI + memory[0].values[pos] + memory[0].values[pos + 1]*0x100;
            return addr;
            }
        case 0b010:
            {
            addr = sregs.BP + sregs.SI + memory[0].values[pos] + memory[0].values[pos + 1]*0x100;
            return addr;
            }
        case 0b011:
            {
            addr = sregs.BP + sregs.DI + memory[0].values[pos] + memory[0].values[pos + 1]*0x100;
            return addr;
            }
        case 0b100:
            {
            addr = sregs.SI + memory[0].values[pos] + memory[0].values[pos + 1]*0x100;
            return addr;
            }
        case 0b101:
            {
            addr = sregs.DI + memory[0].values[pos] + memory[0].values[pos + 1]*0x100;
            return addr;
            }
        case 0b110:
            {
            addr = sregs.BP + memory[0].values[pos] + memory[0].values[pos + 1]*0x100;
            return addr;
            }
        case 0b111:
            {
            addr = sregs.BX + memory[0].values[pos] + memory[0].values[pos + 1]*0x100;
            return addr;
            }
    }
    exit_angrily
}

uint8_t* reg8_decoder(Survivor survivor[static 1], uint8_t byte) {
    uint8_t *reg;
    switch (byte) {
        case 0b000: {
            reg = (uint8_t * ) & (sregs.AX);
            return reg;
        }
        case 0b001: {
            reg = (uint8_t * ) & (sregs.CX);
            return reg;
        }
        case 0b010: {
            reg = (uint8_t * ) & (sregs.DX);
            return reg;
        }
        case 0b011: {
            reg = (uint8_t * ) & (sregs.BX);
            return reg;
        }
        case 0b100: {
            reg = (uint8_t * ) & (sregs.AX) + 1;
            return reg;
        }
        case 0b101: {
            reg = (uint8_t * ) & (sregs.CX) + 1;
            return reg;
        }
        case 0b110: {
            reg = (uint8_t * ) & (sregs.DX) + 1;
            return reg;
        }
        case 0b111: {
            reg = (uint8_t * ) & (sregs.BX) + 1;
            return reg;
        }
    }
    exit_angrily
}

uint16_t* reg16_decoder(Survivor survivor[static 1], uint8_t byte) {
    uint16_t* reg;
    switch (byte) {
        case 0b000: {
            reg = &(sregs.AX);
            return reg;
        }
        case 0b001: {
            reg = &(sregs.CX);
            return reg;
        }
        case 0b010: {
            reg = &(sregs.DX);
            return reg;
        }
        case 0b011: {
            reg = &(sregs.BX);
            return reg;
        }
        case 0b100: {
            reg = &(sregs.SP);
            return reg;
        }
        case 0b101: {
            reg = &(sregs.BP);
            return reg;
        }
        case 0b110: {
            reg = &(sregs.SI);
            return reg;
        }
        case 0b111: {
            reg = &(sregs.DI);
            return reg;
        }
    }
    exit_angrily
}

// General function to add two addresses while taking care of Flags
// might be different from og implementation, because we implemented AF.
void general_add(Survivor survivor[static 1], bool is_16_bit, uint8_t significant_from[static 1], uint8_t* insignificant_from,
                 uint8_t significant_to[static 1], uint8_t* insignificant_to)
{
    if (is_16_bit) {
        uint16_t num_a = (*significant_from<<8) + *insignificant_from;
        uint16_t num_b = (*significant_to<<8) + *insignificant_to;

        uint16_t result = num_a + num_b;

        uint16_t flags_to_update = flags_16_bit_add(num_a, num_b);
        update_specific_flags(survivor, sregs.Flags, flags_to_update, 0x08D5);

        *significant_to = (result&0xFF00)>>8;
        *insignificant_to = result&0x00FF;

    }
    else {
        uint8_t result = *significant_to + *significant_from;

        uint16_t flags_to_update = flags_8_bit_add(*significant_to, *significant_from);
        update_specific_flags(survivor, sregs.Flags, flags_to_update, 0x08D5);


        *significant_to = result;
    }
}

void update_specific_flags(Survivor survivor[static 1], uint16_t og_flags, uint16_t flags_after_change, uint16_t mask) {
    uint16_t difference = og_flags ^ flags_after_change;
    difference &= mask;
    sregs.Flags = (og_flags ^ difference);
}

// might be a difference between our implementation and official implementation here.
// in the official implementation there is an exception when we push or pop from the end of the stack,
// here we just loop to the beginning of the segment.
bool general_push(Survivor survivor[static 1], uint16_t shared_memory, uint16_t reg[static 1])
{
    sregs.SP -= 2;
    uint16_t destination = sregs.SP;

    uint16_t segment = ((destination+0x10*sregs.SS) & 0xF0000) >> 16; // stores the actual segment that will be written to, making sure it is legal
    if (segment != 0 && segment != survivor->stack_id && segment != shared_memory) {return false;}

    ((char*)memory)[(uint32_t) destination + 0x10*sregs.SS] = (*reg & 0xFF00) >> 8; // set first byte

    destination--;
    ((char*)memory)[(uint32_t) destination + 0x10*sregs.SS] += *reg & 0xFF; // set second byte

    sregs.IP += 1;

    return true;

}

bool general_pop(Survivor survivor[static 1], uint16_t shared_memory, uint16_t reg[static 1])
{
    uint16_t address = sregs.SP;

    uint16_t segment = ((address+0x10*sregs.SS) & 0xF0000) >> 16;
    if (segment != 0 && segment != survivor->stack_id && segment != shared_memory) {return false;}

    *reg = ((char*)memory)[(uint32_t) address + 0x10*sregs.SS] << 8; // Might look unnecessarily complicated, but is necessary to enable loopback

    address--;
    *(char*)reg = ((char*)memory)[(uint32_t) address + 0x10*sregs.SS];

    sregs.SP += 2;

    sregs.IP += 1;

    return true;
}
#include "globals.h"
#include "structs_libraries_and_macros.h"
#include "opcode_helper_functions.h"

bool get_virtual_address(uint8_t ip_progress[static 1], uint8_t* destination[static 1],
                         uint16_t destination_virtual_addr[static 1], Survivor survivor[static 1],
                         uint8_t address_byte[static 1], uint16_t pos, uint16_t segment_register_virtual_addr[static 1])
{
    switch (*address_byte >> 6) {
        case 0b00: {
            *destination_virtual_addr = address_decoder_mode00(survivor, *address_byte & 0b00000111, pos, segment_register_virtual_addr, ip_progress);

            *ip_progress += 2;
            return true;
        }
        case 0b01: {
            *destination_virtual_addr = address_decoder_mode01(survivor, *address_byte & 0b00000111, segment_register_virtual_addr, pos);

            *ip_progress += 3;
            return true;
        }
        case 0b10: {
            *destination_virtual_addr = address_decoder_mode10(survivor, *address_byte & 0b00000111, segment_register_virtual_addr, pos);

            *ip_progress += 4;
            return true;
        }
        case 0b11: {
            *destination = reg8_decoder(survivor, (*address_byte & 0b00000111));

            *ip_progress += 2;
            return true;
        }
        default: {
            return false;
        }
    }
}

// assume byte is of the form 0b00000xxx
uint16_t address_decoder_mode00(Survivor survivor[static 1], uint8_t byte, uint16_t pos, uint16_t segment_register_virtual_addr[static 1], uint8_t ip_progress[static 1]) {
    uint16_t addr;
    switch (byte) {
        case 0b000: {
            addr = sregs.BX + sregs.SI;
            *segment_register_virtual_addr = sregs.DS;
            return addr;
        }
        case 0b001: {
            addr = sregs.BX + sregs.DI;
            *segment_register_virtual_addr = sregs.DS;
            return addr;
        }
        case 0b010: {
            addr = sregs.BP + sregs.SI;
            *segment_register_virtual_addr = sregs.SS;
            return addr;
        }
        case 0b011: {
            addr = sregs.BP + sregs.DI;
            *segment_register_virtual_addr = sregs.SS;
            return addr;
        }
        case 0b100: {
            addr = sregs.SI;
            *segment_register_virtual_addr = sregs.SS;
            return addr;
        }
        case 0b101: {
            addr = sregs.DI;
            *segment_register_virtual_addr = sregs.DS;
            return addr;
        }
        case 0b110: {
            addr = memory[0].values[pos] + memory[0].values[pos + 1] * 0x100;
            *segment_register_virtual_addr = sregs.DS;
            *ip_progress += 2;
            return addr;
        }
        case 0b111: {
            addr = sregs.BX;
            *segment_register_virtual_addr = sregs.DS;
            return addr;
        }
    }
    exit_angrily
}

uint16_t address_decoder_mode01(Survivor survivor[static 1], uint8_t byte, uint16_t segment_register_virtual_addr[static 1], uint16_t pos) {
    uint16_t addr;
    switch (byte) {
        case 0b000: {
            addr = sregs.BX + sregs.SI + memory[0].values[pos];
            *segment_register_virtual_addr = sregs.DS;
            return addr;
        }
        case 0b001: {
            addr = sregs.BX + sregs.DI + memory[0].values[pos];
            *segment_register_virtual_addr = sregs.DS;
            return addr;
        }
        case 0b010: {
            addr = sregs.BP + sregs.SI + memory[0].values[pos];
            *segment_register_virtual_addr = sregs.SS;
            return addr;
        }
        case 0b011: {
            addr = sregs.BP + sregs.DI + memory[0].values[pos];
            *segment_register_virtual_addr = sregs.SS;
            return addr;
        }
        case 0b100: {
            addr = sregs.SI + memory[0].values[pos];
            *segment_register_virtual_addr = sregs.DS;
            return addr;
        }
        case 0b101: {
            addr = sregs.DI + memory[0].values[pos];
            *segment_register_virtual_addr = sregs.DS;
            return addr;
        }
        case 0b110: {
            addr = sregs.BP + memory[0].values[pos];
            *segment_register_virtual_addr = sregs.SS;
            return addr;
        }
        case 0b111: {
            addr = sregs.BX + memory[0].values[pos];
            *segment_register_virtual_addr = sregs.DS;
            return addr;
        }
    }
    exit_angrily
}


uint16_t address_decoder_mode10(Survivor survivor[static 1], uint8_t byte, uint16_t segment_register_virtual_addr[static 1], uint16_t pos) {
    uint16_t addr;
    switch (byte) {
        case 0b000:
            {
            addr = sregs.BX + sregs.SI + memory[0].values[pos] + memory[0].values[pos + 1] * 0x100;
                *segment_register_virtual_addr = sregs.DS;
            return addr;
            }
        case 0b001:
            {
            addr = sregs.BX + sregs.DI + memory[0].values[pos] + memory[0].values[pos + 1]*0x100;
            *segment_register_virtual_addr = sregs.DS;
            return addr;
            }
        case 0b010:
            {
            addr = sregs.BP + sregs.SI + memory[0].values[pos] + memory[0].values[pos + 1]*0x100;
            *segment_register_virtual_addr = sregs.SS;
            return addr;
            }
        case 0b011:
            {
            addr = sregs.BP + sregs.DI + memory[0].values[pos] + memory[0].values[pos + 1]*0x100;
            *segment_register_virtual_addr = sregs.SS;
            return addr;
            }
        case 0b100:
            {
            addr = sregs.SI + memory[0].values[pos] + memory[0].values[pos + 1]*0x100;
            *segment_register_virtual_addr = sregs.DS;
            return addr;
            }
        case 0b101:
            {
            addr = sregs.DI + memory[0].values[pos] + memory[0].values[pos + 1]*0x100;
            *segment_register_virtual_addr = sregs.DS;
            return addr;
            }
        case 0b110:
            {
            addr = sregs.BP + memory[0].values[pos] + memory[0].values[pos + 1]*0x100;
            *segment_register_virtual_addr = sregs.SS;
            return addr;
            }
        case 0b111:
            {
            addr = sregs.BX + memory[0].values[pos] + memory[0].values[pos + 1]*0x100;
            *segment_register_virtual_addr = sregs.DS;
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

uint16_t* seg_decoder(Survivor survivor[static 1], uint8_t byte) {
    uint16_t* reg;
    switch (byte & 0b11) {
        case 0b00: {
            reg = &(sregs.ES);
            return reg;
        }
        case 0b01: {
            reg = &(sregs.CS);
            return reg;
        }
        case 0b10: {
            reg = &(sregs.SS);
            return reg;
        }
        case 0b11: {
            reg = &(sregs.DS);
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

        uint16_t result;
        uint_fast16_t flags_to_update;

        asm (   "add %3, %2\r\n\t"
                "mov %0, %3\r\n\t"
                "pushf\r\n\t"
                "pop %1"
                : "=r" (result), "=r" (flags_to_update)
                : "r" (num_a), "r" (num_b)
                );

        update_specific_flags(survivor, sregs.Flags, flags_to_update, 0x08D5);

        *significant_to = (result&0xFF00)>>8;
        *insignificant_to = result&0x00FF;

    }
    else {
        uint8_t result;
        uint_fast16_t flags_to_update;

        asm (   "add %3, %2\r\n\t"
                "mov %0, %3\r\n\t"
                "pushf\r\n\t"
                "pop %1"

                : "=r" (result), "=r" (flags_to_update)
                : "r" (*significant_from), "r" (*significant_to)
                );

        *significant_to = result;

        update_specific_flags(survivor, sregs.Flags, flags_to_update, 0x08D5);
    }
}

void general_or(Survivor survivor[static 1], bool is_16_bit, uint8_t significant_from[static 1], uint8_t* insignificant_from,
                uint8_t significant_to[static 1], uint8_t* insignificant_to)
{
    if (is_16_bit) {
        uint16_t num_a = (*significant_from<<8) + *insignificant_from;
        uint16_t num_b = (*significant_to<<8) + *insignificant_to;

        uint16_t result;
        uint_fast16_t flags_to_update;

        asm (   "or %3, %2\r\n\t"
                "mov %0, %3\r\n\t"
                "pushf\r\n\t"
                "pop %1"
                : "=r" (result), "=r" (flags_to_update)
                : "r" (num_a), "r" (num_b)
                );

        update_specific_flags(survivor, sregs.Flags, flags_to_update, 0x08D5);

        *significant_to = (result&0xFF00)>>8;
        *insignificant_to = result&0x00FF;

    }
    else {
        uint8_t result;
        uint_fast16_t flags_to_update;

        asm (   "or %3, %2\r\n\t"
                "mov %0, %3\r\n\t"
                "pushf\r\n\t"
                "pop %1"

                : "=r" (result), "=r" (flags_to_update)
                : "r" (*significant_from), "r" (*significant_to)
                );

        *significant_to = result;

        update_specific_flags(survivor, sregs.Flags, flags_to_update, 0x08D5);
    }
}

void general_adc(Survivor survivor[static 1], bool is_16_bit, uint8_t significant_from[static 1], uint8_t* insignificant_from,
                 uint8_t significant_to[static 1], uint8_t* insignificant_to)
{
    if (is_16_bit) {
        uint16_t num_a = (*significant_from<<8) + *insignificant_from;
        uint16_t num_b = (*significant_to<<8) + *insignificant_to;

        uint16_t result;
        uint_fast16_t flags_to_update;

        asm (   "adc %3, %2\r\n\t"
                "mov %0, %3\r\n\t"
                "pushf\r\n\t"
                "pop %1"
                : "=r" (result), "=r" (flags_to_update)
                : "r" (num_a), "r" (num_b)
                );

        update_specific_flags(survivor, sregs.Flags, flags_to_update, 0x08D5);

        *significant_to = (result&0xFF00)>>8;
        *insignificant_to = result&0x00FF;

    }
    else {
        uint8_t result;
        uint_fast16_t flags_to_update;

        asm (   "adc %3, %2\r\n\t"
                "mov %0, %3\r\n\t"
                "pushf\r\n\t"
                "pop %1"

                : "=r" (result), "=r" (flags_to_update)
                : "r" (*significant_from), "r" (*significant_to)
                );

        *significant_to = result;

        update_specific_flags(survivor, sregs.Flags, flags_to_update, 0x08D5);
    }
}

void general_sbb(Survivor survivor[static 1], bool is_16_bit, uint8_t significant_from[static 1], uint8_t* insignificant_from,
                 uint8_t significant_to[static 1], uint8_t* insignificant_to)
{
    if (is_16_bit) {
        uint16_t num_a = (*significant_from<<8) + *insignificant_from;
        uint16_t num_b = (*significant_to<<8) + *insignificant_to;

        uint16_t result;
        uint_fast16_t flags_to_update;

        asm (   "sbb %3, %2\r\n\t"
                "mov %0, %3\r\n\t"
                "pushf\r\n\t"
                "pop %1"
                : "=r" (result), "=r" (flags_to_update)
                : "r" (num_a), "r" (num_b)
                );

        update_specific_flags(survivor, sregs.Flags, flags_to_update, 0x08D5);

        *significant_to = (result&0xFF00)>>8;
        *insignificant_to = result&0x00FF;

    }
    else {
        uint8_t result;
        uint_fast16_t flags_to_update;

        asm (   "sbb %3, %2\r\n\t"
                "mov %0, %3\r\n\t"
                "pushf\r\n\t"
                "pop %1"

                : "=r" (result), "=r" (flags_to_update)
                : "r" (*significant_from), "r" (*significant_to)
                );

        *significant_to = result;

        update_specific_flags(survivor, sregs.Flags, flags_to_update, 0x08D5);
    }
}

void general_and(Survivor survivor[static 1], bool is_16_bit, uint8_t significant_from[static 1], uint8_t* insignificant_from,
                 uint8_t significant_to[static 1], uint8_t* insignificant_to)
{
    if (is_16_bit) {
        uint16_t num_a = (*significant_from<<8) + *insignificant_from;
        uint16_t num_b = (*significant_to<<8) + *insignificant_to;

        uint16_t result;
        uint_fast16_t flags_to_update;

        asm (   "and %3, %2\r\n\t"
                "mov %0, %3\r\n\t"
                "pushf\r\n\t"
                "pop %1"
                : "=r" (result), "=r" (flags_to_update)
                : "r" (num_a), "r" (num_b)
                );

        update_specific_flags(survivor, sregs.Flags, flags_to_update, 0x08D5);

        *significant_to = (result&0xFF00)>>8;
        *insignificant_to = result&0x00FF;

    }
    else {
        uint8_t result;
        uint_fast16_t flags_to_update;

        asm (   "and %3, %2\r\n\t"
                "mov %0, %3\r\n\t"
                "pushf\r\n\t"
                "pop %1"

                : "=r" (result), "=r" (flags_to_update)
                : "r" (*significant_from), "r" (*significant_to)
                );

        *significant_to = result;

        update_specific_flags(survivor, sregs.Flags, flags_to_update, 0x08D5);
    }
}

void general_sub(Survivor survivor[static 1], bool is_16_bit, uint8_t significant_from[static 1], uint8_t* insignificant_from,
                 uint8_t significant_to[static 1], uint8_t* insignificant_to)
{
    if (is_16_bit) {
        uint16_t num_a = (*significant_from<<8) + *insignificant_from;
        uint16_t num_b = (*significant_to<<8) + *insignificant_to;

        uint16_t result;
        uint_fast16_t flags_to_update;

        asm (   "sub %3, %2\r\n\t"
                "mov %0, %3\r\n\t"
                "pushf\r\n\t"
                "pop %1"
                : "=r" (result), "=r" (flags_to_update)
                : "r" (num_a), "r" (num_b)
                );

        update_specific_flags(survivor, sregs.Flags, flags_to_update, 0x08D5);

        *significant_to = (result&0xFF00)>>8;
        *insignificant_to = result&0x00FF;

    }
    else {
        uint8_t result;
        uint_fast16_t flags_to_update;

        asm (   "sub %3, %2\r\n\t"
                "mov %0, %3\r\n\t"
                "pushf\r\n\t"
                "pop %1"

                : "=r" (result), "=r" (flags_to_update)
                : "r" (*significant_from), "r" (*significant_to)
                );

        *significant_to = result;

        update_specific_flags(survivor, sregs.Flags, flags_to_update, 0x08D5);
    }
}

void general_xor(Survivor survivor[static 1], bool is_16_bit, uint8_t significant_from[static 1], uint8_t* insignificant_from,
                 uint8_t significant_to[static 1], uint8_t* insignificant_to)
{
    if (is_16_bit) {
        uint16_t num_a = (*significant_from<<8) + *insignificant_from;
        uint16_t num_b = (*significant_to<<8) + *insignificant_to;

        uint16_t result;
        uint_fast16_t flags_to_update;

        asm (   "xor %3, %2\r\n\t"
                "mov %0, %3\r\n\t"
                "pushf\r\n\t"
                "pop %1"
                : "=r" (result), "=r" (flags_to_update)
                : "r" (num_a), "r" (num_b)
                );

        update_specific_flags(survivor, sregs.Flags, flags_to_update, 0x08D5);

        *significant_to = (result&0xFF00)>>8;
        *insignificant_to = result&0x00FF;

    }
    else {
        uint8_t result;
        uint_fast16_t flags_to_update;

        asm (   "xor %3, %2\r\n\t"
                "mov %0, %3\r\n\t"
                "pushf\r\n\t"
                "pop %1"

                : "=r" (result), "=r" (flags_to_update)
                : "r" (*significant_from), "r" (*significant_to)
                );

        *significant_to = result;

        update_specific_flags(survivor, sregs.Flags, flags_to_update, 0x08D5);
    }
}

void general_cmp(Survivor survivor[static 1], bool is_16_bit, uint8_t significant_from[static 1], uint8_t* insignificant_from,
                 uint8_t significant_to[static 1], uint8_t* insignificant_to)
{
    if (is_16_bit) {
        uint16_t num_a = (*significant_from<<8) + *insignificant_from;
        uint16_t num_b = (*significant_to<<8) + *insignificant_to;

        uint16_t result;
        uint_fast16_t flags_to_update;

        asm (   "cmp %3, %2\r\n\t"
                "mov %0, %3\r\n\t"
                "pushf\r\n\t"
                "pop %1"
                : "=r" (result), "=r" (flags_to_update)
                : "r" (num_a), "r" (num_b)
                );

        update_specific_flags(survivor, sregs.Flags, flags_to_update, 0x08D5);

        *significant_to = (result&0xFF00)>>8;
        *insignificant_to = result&0x00FF;

    }
    else {
        uint8_t result;
        uint_fast16_t flags_to_update;

        asm (   "cmp %3, %2\r\n\t"
                "mov %0, %3\r\n\t"
                "pushf\r\n\t"
                "pop %1"

                : "=r" (result), "=r" (flags_to_update)
                : "r" (*significant_from), "r" (*significant_to)
                );

        *significant_to = result;

        update_specific_flags(survivor, sregs.Flags, flags_to_update, 0x08D5);
    }
}

void general_mov(Survivor survivor[static 1], bool is_16_bit, uint8_t significant_from[static 1], uint8_t* insignificant_from,
                 uint8_t significant_to[static 1], uint8_t* insignificant_to)
{
    if (is_16_bit) {
        uint16_t num_a = (*significant_from<<8) + *insignificant_from;
        uint16_t num_b = (*significant_to<<8) + *insignificant_to;

        uint16_t result = num_b;

        asm (   "mov %2, %1\r\n\t"
                "mov %0, %2"
                : "=r" (result)
                : "r" (num_a), "r" (num_b)
                );

        *significant_to = (result&0xFF00)>>8;
        *insignificant_to = result&0x00FF;

    }
    else {
        uint8_t result = *significant_to;

        asm (   "mov %2, %1\r\n\t"
                "mov %0, %2"
                : "=r" (result)
                : "r" (*significant_from), "r" (*significant_to)
                );

        *significant_to = result;
    }
}

void general_test(Survivor survivor[static 1], bool is_16_bit, uint8_t significant_from[static 1], uint8_t* insignificant_from,
                  uint8_t significant_to[static 1], uint8_t* insignificant_to)
{
    if (is_16_bit) {
        uint16_t num_a = (*significant_from<<8) + *insignificant_from;
        uint16_t num_b = (*significant_to<<8) + *insignificant_to;

        uint16_t result;
        uint_fast16_t flags_to_update;

        asm (   "test %3, %2\r\n\t"
                "mov %0, %3\r\n\t"
                "pushf\r\n\t"
                "pop %1"
                : "=r" (result), "=r" (flags_to_update)
                : "r" (num_a), "r" (num_b)
                );

        update_specific_flags(survivor, sregs.Flags, flags_to_update, 0x08D5);

        *significant_to = (result&0xFF00)>>8;
        *insignificant_to = result&0x00FF;

    }
    else {
        uint8_t result;
        uint_fast16_t flags_to_update;

        asm (   "test %3, %2\r\n\t"
                "mov %0, %3\r\n\t"
                "pushf\r\n\t"
                "pop %1"

                : "=r" (result), "=r" (flags_to_update)
                : "r" (*significant_from), "r" (*significant_to)
                );

        *significant_to = result;

        update_specific_flags(survivor, sregs.Flags, flags_to_update, 0x08D5);
    }
}

void general_rol(Survivor survivor[static 1], bool is_16_bit, uint8_t significant_from[static 1], uint8_t* insignificant_from,
                  uint8_t significant_to[static 1], uint8_t* insignificant_to)
{
    if (is_16_bit) {
        uint8_t num_a = (*significant_from<<8) + *insignificant_from;
        uint16_t num_b = (*significant_to<<8) + *insignificant_to;

        uint16_t result;
        uint_fast16_t flags_to_update;

        asm (   "mov cl, %2\r\n\t"
                "rol %3, cl\r\n\t"
                "mov %0, %3\r\n\t"
                "pushf\r\n\t"
                "pop %1"
                : "=r" (result), "=r" (flags_to_update)
                : "r" (num_a), "r" (num_b)
                : "cl"
                );

        update_specific_flags(survivor, sregs.Flags, flags_to_update, 0x08D5);

        *significant_to = (result&0xFF00)>>8;
        *insignificant_to = result&0x00FF;

    }
    else {
        uint8_t result;
        uint_fast16_t flags_to_update;

        asm (   "mov cl, %2\r\n\t"
                "rol %3, cl\r\n\t"
                "mov %0, %3\r\n\t"
                "pushf\r\n\t"
                "pop %1"

                : "=r" (result), "=r" (flags_to_update)
                : "r" (*significant_from), "r" (*significant_to)
                : "cl"
                );

        *significant_to = result;

        update_specific_flags(survivor, sregs.Flags, flags_to_update, 0x08D5);
    }
}

void general_ror(Survivor survivor[static 1], bool is_16_bit, uint8_t significant_from[static 1], uint8_t* insignificant_from,
                 uint8_t significant_to[static 1], uint8_t* insignificant_to)
{
    if (is_16_bit) {
        uint8_t num_a = (*significant_from<<8) + *insignificant_from;
        uint16_t num_b = (*significant_to<<8) + *insignificant_to;

        uint16_t result;
        uint_fast16_t flags_to_update;

        asm (   "mov cl, %2\r\n\t"
                "ror %3, cl\r\n\t"
                "mov %0, %3\r\n\t"
                "pushf\r\n\t"
                "pop %1"
                : "=r" (result), "=r" (flags_to_update)
                : "r" (num_a), "r" (num_b)
                : "cl"
                );

        update_specific_flags(survivor, sregs.Flags, flags_to_update, 0x08D5);

        *significant_to = (result&0xFF00)>>8;
        *insignificant_to = result&0x00FF;

    }
    else {
        uint8_t result;
        uint_fast16_t flags_to_update;

        asm (   "mov cl, %2\r\n\t"
                "ror %3, cl\r\n\t"
                "mov %0, %3\r\n\t"
                "pushf\r\n\t"
                "pop %1"

                : "=r" (result), "=r" (flags_to_update)
                : "r" (*significant_from), "r" (*significant_to)
                : "cl"
                );

        *significant_to = result;

        update_specific_flags(survivor, sregs.Flags, flags_to_update, 0x08D5);
    }
}

void general_rcl(Survivor survivor[static 1], bool is_16_bit, uint8_t significant_from[static 1], uint8_t* insignificant_from,
                 uint8_t significant_to[static 1], uint8_t* insignificant_to)
{
    if (is_16_bit) {
        uint8_t num_a = (*significant_from<<8) + *insignificant_from;
        uint16_t num_b = (*significant_to<<8) + *insignificant_to;

        uint16_t result;
        uint_fast16_t flags_to_update;

        asm (   "mov cl, %2\r\n\t"
                "rcl %3, cl\r\n\t"
                "mov %0, %3\r\n\t"
                "pushf\r\n\t"
                "pop %1"
                : "=r" (result), "=r" (flags_to_update)
                : "r" (num_a), "r" (num_b)
                : "cl"
                );

        update_specific_flags(survivor, sregs.Flags, flags_to_update, 0x08D5);

        *significant_to = (result&0xFF00)>>8;
        *insignificant_to = result&0x00FF;

    }
    else {
        uint8_t result;
        uint_fast16_t flags_to_update;

        asm (   "mov cl, %2\r\n\t"
                "rcl %3, cl\r\n\t"
                "mov %0, %3\r\n\t"
                "pushf\r\n\t"
                "pop %1"

                : "=r" (result), "=r" (flags_to_update)
                : "r" (*significant_from), "r" (*significant_to)
                : "cl"
                );

        *significant_to = result;

        update_specific_flags(survivor, sregs.Flags, flags_to_update, 0x08D5);
    }
}

void general_rcr(Survivor survivor[static 1], bool is_16_bit, uint8_t significant_from[static 1], uint8_t* insignificant_from,
                 uint8_t significant_to[static 1], uint8_t* insignificant_to)
{
    if (is_16_bit) {
        uint8_t num_a = (*significant_from<<8) + *insignificant_from;
        uint16_t num_b = (*significant_to<<8) + *insignificant_to;

        uint16_t result;
        uint_fast16_t flags_to_update;

        asm (   "mov cl, %2\r\n\t"
                "rcr %3, cl\r\n\t"
                "mov %0, %3\r\n\t"
                "pushf\r\n\t"
                "pop %1"
                : "=r" (result), "=r" (flags_to_update)
                : "r" (num_a), "r" (num_b)
                : "cl"
                );

        update_specific_flags(survivor, sregs.Flags, flags_to_update, 0x08D5);

        *significant_to = (result&0xFF00)>>8;
        *insignificant_to = result&0x00FF;

    }
    else {
        uint8_t result;
        uint_fast16_t flags_to_update;

        asm (   "mov cl, %2\r\n\t"
                "rcr %3, cl\r\n\t"
                "mov %0, %3\r\n\t"
                "pushf\r\n\t"
                "pop %1"

                : "=r" (result), "=r" (flags_to_update)
                : "r" (*significant_from), "r" (*significant_to)
                : "cl"
                );

        *significant_to = result;

        update_specific_flags(survivor, sregs.Flags, flags_to_update, 0x08D5);
    }
}

void general_shl(Survivor survivor[static 1], bool is_16_bit, uint8_t significant_from[static 1], uint8_t* insignificant_from,
                 uint8_t significant_to[static 1], uint8_t* insignificant_to)
{
    if (is_16_bit) {
        uint8_t num_a = (*significant_from<<8) + *insignificant_from;
        uint16_t num_b = (*significant_to<<8) + *insignificant_to;

        uint16_t result;
        uint_fast16_t flags_to_update;

        asm (   "mov cl, %2\r\n\t"
                "shl %3, cl\r\n\t"
                "mov %0, %3\r\n\t"
                "pushf\r\n\t"
                "pop %1"
                : "=r" (result), "=r" (flags_to_update)
                : "r" (num_a), "r" (num_b)
                : "cl"
                );

        update_specific_flags(survivor, sregs.Flags, flags_to_update, 0x08D5);

        *significant_to = (result&0xFF00)>>8;
        *insignificant_to = result&0x00FF;

    }
    else {
        uint8_t result;
        uint_fast16_t flags_to_update;

        asm (   "mov cl, %2\r\n\t"
                "shl %3, cl\r\n\t"
                "mov %0, %3\r\n\t"
                "pushf\r\n\t"
                "pop %1"

                : "=r" (result), "=r" (flags_to_update)
                : "r" (*significant_from), "r" (*significant_to)
                : "cl"
                );

        *significant_to = result;

        update_specific_flags(survivor, sregs.Flags, flags_to_update, 0x08D5);
    }
}

void general_shr(Survivor survivor[static 1], bool is_16_bit, uint8_t significant_from[static 1], uint8_t* insignificant_from,
                 uint8_t significant_to[static 1], uint8_t* insignificant_to)
{
    if (is_16_bit) {
        uint8_t num_a = (*significant_from<<8) + *insignificant_from;
        uint16_t num_b = (*significant_to<<8) + *insignificant_to;

        uint16_t result;
        uint_fast16_t flags_to_update;

        asm (   "mov cl, %2\r\n\t"
                "shr %3, cl\r\n\t"
                "mov %0, %3\r\n\t"
                "pushf\r\n\t"
                "pop %1"
                : "=r" (result), "=r" (flags_to_update)
                : "r" (num_a), "r" (num_b)
                : "cl"
                );

        update_specific_flags(survivor, sregs.Flags, flags_to_update, 0x08D5);

        *significant_to = (result&0xFF00)>>8;
        *insignificant_to = result&0x00FF;

    }
    else {
        uint8_t result;
        uint_fast16_t flags_to_update;

        asm (   "mov cl, %2\r\n\t"
                "shr %3, cl\r\n\t"
                "mov %0, %3\r\n\t"
                "pushf\r\n\t"
                "pop %1"

                : "=r" (result), "=r" (flags_to_update)
                : "r" (*significant_from), "r" (*significant_to)
                : "cl"
                );

        *significant_to = result;

        update_specific_flags(survivor, sregs.Flags, flags_to_update, 0x08D5);
    }
}

void general_sar(Survivor survivor[static 1], bool is_16_bit, uint8_t significant_from[static 1], uint8_t* insignificant_from,
                 uint8_t significant_to[static 1], uint8_t* insignificant_to)
{
    if (is_16_bit) {
        uint8_t num_a = (*significant_from<<8) + *insignificant_from;
        uint16_t num_b = (*significant_to<<8) + *insignificant_to;

        uint16_t result;
        uint_fast16_t flags_to_update;

        asm (   "mov cl, %2\r\n\t"
                "sar %3, cl\r\n\t"
                "mov %0, %3\r\n\t"
                "pushf\r\n\t"
                "pop %1"
                : "=r" (result), "=r" (flags_to_update)
                : "r" (num_a), "r" (num_b)
                : "cl"
                );

        update_specific_flags(survivor, sregs.Flags, flags_to_update, 0x08D5);

        *significant_to = (result&0xFF00)>>8;
        *insignificant_to = result&0x00FF;

    }
    else {
        uint8_t result;
        uint_fast16_t flags_to_update;

        asm (   "mov cl, %2\r\n\t"
                "sar %3, cl\r\n\t"
                "mov %0, %3\r\n\t"
                "pushf\r\n\t"
                "pop %1"

                : "=r" (result), "=r" (flags_to_update)
                : "r" (*significant_from), "r" (*significant_to)
                : "cl"
                );

        *significant_to = result;

        update_specific_flags(survivor, sregs.Flags, flags_to_update, 0x08D5);
    }
}

void general_xchg(Survivor survivor[static 1], bool is_16_bit, uint8_t significant_from[static 1], uint8_t* insignificant_from,
                  uint8_t significant_to[static 1], uint8_t* insignificant_to)
{
    uint8_t tmp = *significant_to;
    *significant_to = *significant_from;
    *significant_from = tmp;

    if (is_16_bit) {
        tmp = *insignificant_to;
        *insignificant_to = *insignificant_from;
        *insignificant_from = tmp;
    }
}

void general_mul(Survivor survivor[static 1], bool is_16_bit, uint8_t significant_from[static 1], uint8_t* insignificant_from)
{
    if (is_16_bit) {
        uint16_t num_a = (*significant_from<<8) + *insignificant_from;

        uint_fast16_t flags_to_update;

        asm (   "mov ax, %3\r\n\t"
                "mul %4\r\n\t"
                "mov %0, ax\r\n\t"
                "mov %1, dx\r\n\t"
                "pushf\r\n\t"
                "pop %2"
                : "=r" (sregs.AX), "=r" (sregs.DX), "=r" (flags_to_update)
                : "r" (sregs.AX), "r" (num_a)
                : "ax", "dx"
                );

        update_specific_flags(survivor, sregs.Flags, flags_to_update, 0x08D5);
    }
    else {
        uint8_t num_a = *significant_from;

        uint_fast16_t flags_to_update;

        asm (   "mov al, %2\r\n\t"
                "mul %3\r\n\t"
                "mov %0, ax\r\n\t"
                "pushf\r\n\t"
                "pop %1"
                : "=r" (sregs.AX), "=r" (flags_to_update)
                : "r" (*(uint8_t*)&sregs.AX), "r" (num_a)
                : "al", "ax"
                );

        update_specific_flags(survivor, sregs.Flags, flags_to_update, 0x08D5);
    }
}

void general_div(Survivor survivor[static 1], bool is_16_bit, uint8_t significant_from[static 1], uint8_t* insignificant_from)
// This function assumes divisor is not zero!
{
    if (is_16_bit) {
        uint16_t num_a = (*significant_from<<8) + *insignificant_from;

        uint_fast16_t flags_to_update;

        asm (   "mov ax, %3\r\n\t"
                "mov dx, %5\r\n\t"
                "div %4\r\n\t"
                "mov %0, ax\r\n\t"
                "mov %1, dx\r\n\t"
                "pushf\r\n\t"
                "pop %2"
                : "=r" (sregs.AX), "=r" (sregs.DX), "=r" (flags_to_update)
                : "r" (sregs.AX), "r" (num_a), "r" (sregs.DX)
                : "ax", "dx"
                );

        update_specific_flags(survivor, sregs.Flags, flags_to_update, 0x08D5);
    }
    else {
        uint8_t num_a = *significant_from;

        uint_fast16_t flags_to_update;

        asm (   "mov ax, %2\r\n\t"
                "div %3\r\n\t"
                "mov %0, ax\r\n\t"
                "pushf\r\n\t"
                "pop %1"
                : "=r" (sregs.AX), "=r" (flags_to_update)
                : "r" (sregs.AX), "r" (num_a)
                : "al", "ax"
                );

        update_specific_flags(survivor, sregs.Flags, flags_to_update, 0x08D5);
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
bool general_push(Survivor survivor[static 1], uint16_t shared_memory, uint16_t reg[static 1]) // Push *
{
    sregs.SP -= 2;
    uint16_t destination = sregs.SP;

    uint16_t segment = ((uint32_t) (destination+0x10*sregs.SS) >> 16); // stores the actual segment that will be written to, making sure it is legal
    if (segment != 0 && segment != survivor->stack_id && segment != shared_memory) {return false;}

    ((uint8_t*) memory)[(uint32_t) destination + 0x10*sregs.SS] = *reg & 0xFF; // set first byte

    destination++;
    ((uint8_t*) memory)[(uint32_t) destination + 0x10*sregs.SS] = (*reg & 0xFF00) >> 8; // set second byte

    sregs.IP += 1;

    return true;

}

bool general_pop(Survivor survivor[static 1], uint16_t shared_memory, uint16_t reg[static 1]) // Pop *
{
    uint16_t address = sregs.SP;

    uint16_t segment = ((uint32_t) (address+0x10*sregs.SS) >> 16);
    if (segment != 0 && segment != survivor->stack_id && segment != shared_memory) {return false;}

    address++;
    *reg = ((uint8_t*) memory)[(uint32_t) address + 0x10*sregs.SS] << 8; // Might look unnecessarily complicated, but is necessary to enable loopback

    address--;
    *(uint8_t*)reg = ((uint8_t*) memory)[(uint32_t) address + 0x10*sregs.SS];

    sregs.SP += 2;

    sregs.IP += 1;

    return true;
}

bool general_op_0(Survivor survivor[static 1], uint16_t shared_memory, operation_ptr general_op) // OP [X], reg8
{
    uint8_t address_byte = memory[0].values[(sregs.IP + 10*sregs.CS + 1) & 0xFFFF];
    uint16_t pos = sregs.IP + 10*sregs.CS + 2;
    uint8_t* address;
    address = reg8_decoder(survivor, (address_byte & 0b00111000) >> 3);
    uint8_t ip_progress = 0;

    uint16_t destination_virtual_addr = 0;
    uint16_t segment_register_virtual_addr = 0;
    uint8_t* destination = 0;

    if (!get_virtual_address(&ip_progress, &destination, &destination_virtual_addr, survivor, &address_byte, pos, &segment_register_virtual_addr)) {return false;}

    if (destination == 0) {
        uint16_t segment = ((uint32_t) (destination_virtual_addr + 0x10 * segment_register_virtual_addr) >> 16);
        if (segment != 0 && segment != survivor->stack_id && segment != shared_memory) {return false;}

       destination = (uint8_t*)&((uint8_t*) memory)[(uint32_t) destination_virtual_addr + segment_register_virtual_addr * 0x10];
    }

    general_op(survivor, 0, address, 0, destination, 0);

    sregs.IP += ip_progress;
    return true;
}

// Might be a difference between our implementation and official implementation here.
// In the official implementation the shared memory loops back to stack, here it loops back to the shared memory.
// in the official implementation the loopback works differently for 16-bit registers.
// Unlike official implementation, here there is an exploit that allows survivors to access the first byte of
// another survivor's private section.
bool general_op_1(Survivor survivor[static 1], uint16_t shared_memory, operation_ptr general_op) // OP [X], reg16
{

    uint8_t address_byte = memory[0].values[(sregs.IP + 10*sregs.CS + 1) & 0xFFFF];
    uint16_t pos = sregs.IP + 10*sregs.CS + 2;

    uint8_t* significant_address,* insignificant_address;

    insignificant_address = (uint8_t*)reg16_decoder(survivor, (address_byte & 0b00111000) >> 3);
    significant_address = insignificant_address + 1;

    uint8_t ip_progress = 0;

    uint16_t destination_virtual_addr = 0;
    uint16_t segment_register_virtual_addr = 0;
    uint8_t* insignificant_destination = 0;
    uint8_t* significant_destination;

    if (!get_virtual_address(&ip_progress, &insignificant_destination, &destination_virtual_addr, survivor, &address_byte, pos, &segment_register_virtual_addr)) {return false;}

    if (insignificant_destination == 0) {
        uint16_t segment = ((uint32_t) (destination_virtual_addr + 0x10 * segment_register_virtual_addr) >> 16);
        if (segment != 0 && segment != survivor->stack_id && segment != shared_memory) {return false;}

        insignificant_destination = &((uint8_t*) memory)[(uint32_t) destination_virtual_addr + segment_register_virtual_addr * 0x10];
        significant_destination = &((uint8_t*) memory)[(uint32_t) ((destination_virtual_addr+1)&0xFFFF) + segment_register_virtual_addr * 0x10];
    }
    else {
        significant_destination = insignificant_destination + 1;
    }


    general_op(survivor, 1, significant_address, insignificant_address, significant_destination, insignificant_destination);

    sregs.IP += ip_progress;
    return true;
}

bool general_op_2(Survivor survivor[static 1], uint16_t shared_memory, operation_ptr general_op) // OP reg8, [X]
{
    uint8_t address_byte = memory[0].values[(sregs.IP + 10*sregs.CS + 1) & 0xFFFF];
    uint16_t pos = sregs.IP + 10*sregs.CS + 2;
    uint8_t* destination;
    destination = reg8_decoder(survivor, (address_byte & 0b00111000) >> 3);
    uint8_t ip_progress = 0;

    uint16_t address_virtual_addr = 0;
    uint16_t segment_register_virtual_addr = 0;
    uint8_t* address = 0;

    if (!get_virtual_address(&ip_progress, &destination, &address_virtual_addr, survivor, &address_byte, pos, &segment_register_virtual_addr)) {return false;}

    if (address == 0) {
        uint16_t segment = ((uint32_t) (address_virtual_addr + 0x10 * segment_register_virtual_addr) >> 16);
        if (segment != 0 && segment != survivor->stack_id && segment != shared_memory) {return false;}

        address = &((uint8_t*) memory)[(uint32_t) address_virtual_addr + segment_register_virtual_addr * 0x10];
    }

    general_op(survivor, 0, address, 0, destination, 0);

    sregs.IP += ip_progress;
    return true;
}

// Might be a difference between our implementation and official implementation here.
// In the official implementation the shared memory loops back to stack, here it loops back to the shared memory.
// in the official implementation the loopback works differently for 16-bit registers.
// Unlike official implementation, here there is an exploit that allows survivors to access the first byte of
// another survivor's private section.
bool general_op_3(Survivor survivor[static 1], uint16_t shared_memory, operation_ptr general_op) // OP reg16, [X]
{

    uint8_t address_byte = memory[0].values[(sregs.IP + 10*sregs.CS + 1) & 0xFFFF];
    uint16_t pos = sregs.IP + 10*sregs.CS + 2;

    uint8_t* significant_destination,* insignificant_destination;

    insignificant_destination = (uint8_t*)reg16_decoder(survivor, (address_byte & 0b00111000) >> 3);
    significant_destination = insignificant_destination + 1;

    uint8_t ip_progress = 0;

    uint16_t address_virtual_addr = 0;
    uint16_t segment_register_virtual_addr = 0;
    uint8_t* insignificant_address = 0;
    uint8_t* significant_address;

    if (!get_virtual_address(&ip_progress, &insignificant_destination, &address_virtual_addr, survivor, &address_byte, pos, &segment_register_virtual_addr)) {return false;}

    if (insignificant_address == 0) {
        uint16_t segment = ((uint32_t) (address_virtual_addr + 0x10 * segment_register_virtual_addr) >> 16);
        if (segment != 0 && segment != survivor->stack_id && segment != shared_memory) {return false;}

        insignificant_address = &((uint8_t*) memory)[(uint32_t) address_virtual_addr + segment_register_virtual_addr * 0x10];
        significant_address = &((uint8_t*) memory)[(uint32_t) ((address_virtual_addr+1)&0xFFFF) + segment_register_virtual_addr * 0x10];
    }
    else {
        significant_destination = insignificant_destination - 1;
    }


    general_op(survivor, 1, significant_address, insignificant_address, significant_destination, insignificant_destination);

    sregs.IP += ip_progress;
    return true;
}

bool general_op_4(Survivor survivor[static 1], uint16_t shared_memory, operation_ptr general_op) // OP AL, imm8
{
    uint8_t address_byte = memory[0].values[(sregs.IP + 10*sregs.CS + 1) & 0xFFFF];
    uint8_t* destination;
    destination = (uint8_t*)&sregs.AX;
    uint8_t ip_progress = 2;

    general_op(survivor, 0, &address_byte, 0, destination, 0);

    sregs.IP += ip_progress;
    return true;
}

bool general_op_5(Survivor survivor[static 1], uint16_t shared_memory, operation_ptr general_op) // OP AX, imm16
{
    uint8_t insignificant_address_byte = memory[0].values[(sregs.IP + 10*sregs.CS + 1) & 0xFFFF];
    uint8_t significant_address_byte = memory[0].values[(sregs.IP + 10*sregs.CS + 2) & 0xFFFF];

    uint8_t* significant_destination,* insignificant_destination;

    insignificant_destination = (uint8_t*)&sregs.AX;
    significant_destination = insignificant_destination + 1;

    uint8_t ip_progress = 3;

    general_op(survivor, 1, &significant_address_byte, &insignificant_address_byte, significant_destination, insignificant_destination);

    sregs.IP += ip_progress;
    return true;
}

bool general_op_6(Survivor survivor[static 1], uint16_t shared_memory, operation_ptr general_op) // OP [X], imm8
{
    uint16_t pos = sregs.IP + 10*sregs.CS + 2;

    uint8_t address_value = memory[0].values[(pos + 2) & 0xFFFF];

    uint8_t ip_progress = 5;

    uint16_t destination_virtual_addr = memory[0].values[pos & 0xFFFF] + 0x100*memory[0].values[(pos + 1) & 0xFFFF];
    uint16_t segment_register_virtual_addr = sregs.DS;

     uint16_t segment = ((uint32_t) (destination_virtual_addr + 0x10 * segment_register_virtual_addr) >> 16);
     if (segment != 0 && segment != survivor->stack_id && segment != shared_memory) {return false;}

     uint8_t* destination = (uint8_t*)&((uint8_t*) memory)[(uint32_t) destination_virtual_addr + segment_register_virtual_addr * 0x10];

    general_op(survivor, 0, &address_value, 0, destination, 0);

    sregs.IP += ip_progress;
    return true;
}

bool general_op_7(Survivor survivor[static 1], uint16_t shared_memory, operation_ptr general_op) // OP [X], imm16
{
    uint16_t pos = sregs.IP + 10*sregs.CS + 2;

    uint8_t* significant_address,* insignificant_address;

    uint16_t address_value = memory[0].values[pos + 2] + 0x100*memory[0].values[pos + 3];
    insignificant_address = (uint8_t*)&address_value;
    significant_address = insignificant_address + 1;

    uint8_t ip_progress = 6;

    uint16_t destination_virtual_addr = memory[0].values[pos + 0] + 0x100*memory[0].values[pos + 1];
    uint16_t segment_register_virtual_addr = sregs.DS;
    uint8_t* insignificant_destination = 0;
    uint8_t* significant_destination;


    uint16_t segment = ((uint32_t) (destination_virtual_addr + 0x10 * segment_register_virtual_addr) >> 16);
    if (segment != 0 && segment != survivor->stack_id && segment != shared_memory) {return false;}

    insignificant_destination = &((uint8_t*) memory)[(uint32_t) destination_virtual_addr + segment_register_virtual_addr * 0x10];
    significant_destination = &((uint8_t*) memory)[(uint32_t) ((destination_virtual_addr+1)&0xFFFF) + segment_register_virtual_addr * 0x10];


    general_op(survivor, 1, significant_address, insignificant_address, significant_destination, insignificant_destination);

    sregs.IP += ip_progress;
    return true;
}

bool general_op_8(Survivor survivor[static 1], uint16_t shared_memory, operation_ptr general_op) // OP [X], sign-extended imm8
{
    uint16_t pos = sregs.IP + 10*sregs.CS + 2;

    uint8_t* significant_address,* insignificant_address;

    uint16_t address_value = memory[0].values[pos + 2];
    uint16_t sign_extension = (address_value & (1 << 7)) * 0xff * 0b10;
    address_value += sign_extension;

    insignificant_address = (uint8_t*)&address_value;
    significant_address = insignificant_address + 1;

    uint8_t ip_progress = 6;

    uint16_t destination_virtual_addr = memory[0].values[pos + 0] + 0x100*memory[0].values[pos + 1];
    uint16_t segment_register_virtual_addr = sregs.DS;
    uint8_t* insignificant_destination = 0;
    uint8_t* significant_destination;


    uint16_t segment = ((uint32_t) (destination_virtual_addr + 0x10 * segment_register_virtual_addr) >> 16);
    if (segment != 0 && segment != survivor->stack_id && segment != shared_memory) {return false;}

    insignificant_destination = &((uint8_t*) memory)[(uint32_t) destination_virtual_addr + segment_register_virtual_addr * 0x10];
    significant_destination = &((uint8_t*) memory)[(uint32_t) ((destination_virtual_addr+1)&0xFFFF) + segment_register_virtual_addr * 0x10];


    general_op(survivor, 1, significant_address, insignificant_address, significant_destination, insignificant_destination);

    sregs.IP += ip_progress;
    return true;
}

bool general_op_9(Survivor survivor[static 1], uint16_t shared_memory, operation_ptr general_op) // OP [X], segment reg16
{

    uint8_t address_byte = memory[0].values[(sregs.IP + 10*sregs.CS + 1) & 0xFFFF];
    uint16_t pos = sregs.IP + 10*sregs.CS + 2;

    uint8_t* significant_address,* insignificant_address;

    insignificant_address = (uint8_t*)seg_decoder(survivor, (address_byte & 0b00111000) >> 3);
    significant_address = insignificant_address + 1;

    uint8_t ip_progress = 0;

    uint16_t destination_virtual_addr = 0;
    uint16_t segment_register_virtual_addr = 0;
    uint8_t* insignificant_destination = 0;
    uint8_t* significant_destination;

    if (!get_virtual_address(&ip_progress, &insignificant_destination, &destination_virtual_addr, survivor, &address_byte, pos, &segment_register_virtual_addr)) {return false;}

    if (insignificant_destination == 0) {
        uint16_t segment = ((uint32_t) (destination_virtual_addr + 0x10 * segment_register_virtual_addr) >> 16);
        if (segment != 0 && segment != survivor->stack_id && segment != shared_memory) {return false;}

        insignificant_destination = &((uint8_t*) memory)[(uint32_t) destination_virtual_addr + segment_register_virtual_addr * 0x10];
        significant_destination = &((uint8_t*) memory)[(uint32_t) ((destination_virtual_addr+1)&0xFFFF) + segment_register_virtual_addr * 0x10];
    }
    else {
        significant_destination = insignificant_destination + 1;
    }


    general_op(survivor, 1, significant_address, insignificant_address, significant_destination, insignificant_destination);

    sregs.IP += ip_progress;
    return true;
}

bool general_op_10(Survivor survivor[static 1], uint16_t shared_memory, operation_ptr general_op) // OP segment reg16, [X]
{

    uint8_t address_byte = memory[0].values[(sregs.IP + 10*sregs.CS + 1) & 0xFFFF];
    uint16_t pos = sregs.IP + 10*sregs.CS + 2;

    uint8_t* significant_destination,* insignificant_destination;

    insignificant_destination = (uint8_t*)seg_decoder(survivor, (address_byte & 0b00111000) >> 3);
    significant_destination = insignificant_destination + 1;

    uint8_t ip_progress = 0;

    uint16_t address_virtual_addr = 0;
    uint16_t segment_register_virtual_addr = 0;
    uint8_t* insignificant_address = 0;
    uint8_t* significant_address;

    if (!get_virtual_address(&ip_progress, &insignificant_address, &address_virtual_addr, survivor, &address_byte, pos, &segment_register_virtual_addr)) {return false;}

    if (insignificant_address == 0) {
        uint16_t segment = ((uint32_t) (address_virtual_addr + 0x10 * segment_register_virtual_addr) >> 16);
        if (segment != 0 && segment != survivor->stack_id && segment != shared_memory) {return false;}

        insignificant_address = &((uint8_t*) memory)[(uint32_t) address_virtual_addr + segment_register_virtual_addr * 0x10];
        significant_address = &((uint8_t*) memory)[(uint32_t) ((address_virtual_addr+1)&0xFFFF) + segment_register_virtual_addr * 0x10];
    }
    else {
        significant_address = insignificant_address + 1;
    }


    general_op(survivor, 1, significant_address, insignificant_address, significant_destination, insignificant_destination);

    sregs.IP += ip_progress;
    return true;
}

bool general_op_11(Survivor survivor[static 1], uint16_t shared_memory, operation_ptr general_op) // OP reg8, imm8
{
    uint8_t address_byte = memory[0].values[(sregs.IP + 10*sregs.CS) & 0xFFFF];
    uint16_t pos = sregs.IP + 10*sregs.CS + 1;
    uint8_t* destination;
    destination = reg8_decoder(survivor, (address_byte & 0b00111000) >> 3);

    uint8_t address_value = memory[0].values[pos];

    general_op(survivor, 0, &address_value, 0, destination, 0);

    sregs.IP += 2;
    return true;
}

bool general_op_12(Survivor survivor[static 1], uint16_t shared_memory, operation_ptr general_op) // OP reg16, imm16
{

    uint8_t address_byte = memory[0].values[(sregs.IP + 10*sregs.CS) & 0xFFFF];
    uint16_t pos = sregs.IP + 10*sregs.CS + 1;

    uint8_t* significant_destination,* insignificant_destination;

    insignificant_destination = (uint8_t*)reg16_decoder(survivor, (address_byte & 0b00111000) >> 3);
    significant_destination = insignificant_destination + 1;

    uint8_t insignificant_address_value = memory[0].values[pos];
    uint8_t significant_address_value = memory[0].values[(pos + 1) & 0xFFFF];

    general_op(survivor, 1, &significant_address_value, &insignificant_address_value, significant_destination, insignificant_destination);

    sregs.IP += 3;
    return true;
}


// it might be the case that warriors can gain access to one byte of forbidden memory, but it is unlikely to not enable some crazy strategy that can win games.
bool general_op_13(Survivor survivor[static 1], uint16_t shared_memory, operation_ptr general_op) // OP byte ptr [X], 1
{
    uint16_t destination_virtual_addr = memory[0].values[(sregs.IP + 10*sregs.CS + 2) & 0xFFFF] + 0x100*memory[0].values[(sregs.IP + 10*sregs.CS + 3) & 0xFFFF];
    uint16_t segment_register_virtual_addr = sregs.DS;

    uint16_t segment = ((uint32_t) (destination_virtual_addr + 0x10 * segment_register_virtual_addr) >> 16);
    if (segment != 0 && segment != survivor->stack_id && segment != shared_memory) {return false;}

    uint8_t* destination = &memory[0].values[(uint32_t) (destination_virtual_addr + 0x10 * segment_register_virtual_addr)];

    uint8_t one = 1;

    general_op(survivor, 0, &one, 0, destination, 0);

    sregs.IP += 4;
    return true;
}

bool general_op_14(Survivor survivor[static 1], uint16_t shared_memory, operation_ptr general_op) // OP word ptr [X], 1
{
    uint16_t destination_virtual_addr = memory[0].values[(sregs.IP + 10*sregs.CS + 2) & 0xFFFF] + 0x100*memory[0].values[(sregs.IP + 10*sregs.CS + 3) & 0xFFFF];
    uint16_t segment_register_virtual_addr = sregs.DS;

    uint16_t real_virtual_addr = (destination_virtual_addr + 0x10 * segment_register_virtual_addr);
    uint16_t segment = ((uint32_t) (destination_virtual_addr + 0x10 * segment_register_virtual_addr) >> 16);
    if (segment != 0 && segment != survivor->stack_id && segment != shared_memory) {return false;}

    uint8_t* insignificant_destination = &memory[0].values[(uint32_t) real_virtual_addr + 0x10000*segment];
    uint8_t* significant_destination = &memory[0].values[(uint32_t) ((real_virtual_addr+1) & 0xFFFF) + 0x10000*segment];

    uint8_t one = 1;
    uint8_t zero = 0;

    general_op(survivor, 1, &zero, &one, significant_destination, insignificant_destination);

    sregs.IP += 4;
    return true;
}

bool general_op_15(Survivor survivor[static 1], uint16_t shared_memory, operation_ptr general_op) // OP byte ptr [X], CL
{
    uint16_t destination_virtual_addr = memory[0].values[(sregs.IP + 10*sregs.CS + 1) & 0xFFFF] + 0x100*memory[0].values[(sregs.IP + 10*sregs.CS + 2) & 0xFFFF];
    uint16_t segment_register_virtual_addr = sregs.DS;

    uint16_t segment = ((uint32_t) (destination_virtual_addr + 0x10 * segment_register_virtual_addr) >> 16);
    if (segment != 0 && segment != survivor->stack_id && segment != shared_memory) {return false;}

    uint8_t* destination = &memory[0].values[(uint32_t) (destination_virtual_addr + 0x10 * segment_register_virtual_addr)];

    general_op(survivor, 0, (uint8_t*)&sregs.CX, 0, destination, 0);

    sregs.IP += 4;
    return true;
}

bool general_op_16(Survivor survivor[static 1], uint16_t shared_memory, operation_ptr general_op) // OP word ptr [X], 1
{
    uint16_t destination_virtual_addr = memory[0].values[(sregs.IP + 10*sregs.CS + 2) & 0xFFFF] + 0x100*memory[0].values[(sregs.IP + 10*sregs.CS + 3) & 0xFFFF];
    uint16_t segment_register_virtual_addr = sregs.DS;

    uint16_t real_virtual_addr = (destination_virtual_addr + 0x10 * segment_register_virtual_addr);
    uint16_t segment = ((uint32_t) (destination_virtual_addr + 0x10 * segment_register_virtual_addr) >> 16);
    if (segment != 0 && segment != survivor->stack_id && segment != shared_memory) {return false;}

    uint8_t* insignificant_destination = &memory[0].values[(uint32_t) real_virtual_addr + 0x10000*segment];
    uint8_t* significant_destination = &memory[0].values[(uint32_t) ((real_virtual_addr+1) & 0xFFFF) + 0x10000*segment];

    general_op(survivor, 1, 0, (uint8_t*)&sregs.CX, significant_destination, insignificant_destination);

    sregs.IP += 4;
    return true;
}

void general_jmp_near_2B_opcode(Survivor survivor[static 1])
{
    sregs.IP += (int8_t) memory[0].values[sregs.IP + 10*sregs.CS + 1] + 2;
}
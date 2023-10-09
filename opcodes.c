#include "structs_libraries_and_macros.h"
#include "globals.h"
#include "opcode_helper_functions.h"

// TODO: add support for flags!!!

bool op_00(Survivor* survivor, uint16_t shared_memory) // ADD [X], reg8
{
    uint8_t address_byte = memory[0].values[survivor->IP+1];
    uint16_t pos = survivor->IP+2;
    uint8_t* address;
    address = reg8_decoder(survivor, (address_byte & 0b00111000) >> 3);
    int ip_progress = 0;

    uint16_t destination_virtual_addr = 0;
    uint8_t* destination = 0;

    switch (address_byte >> 6) {
        case 0b00: {
            destination_virtual_addr = address_decoder_mode00(survivor, address_byte & 0b00000111, pos);

            ip_progress += 2;
            break;
        }
        case 0b01: {
            destination_virtual_addr = address_decoder_mode01(survivor, address_byte & 0b00000111, pos);

            ip_progress += 3;
            break;
        }
        case 0b10: {
            destination_virtual_addr = address_decoder_mode10(survivor, address_byte & 0b00000111, pos);

            ip_progress += 4;
            break;
        }
        case 0b11: {
            destination = reg8_decoder(survivor, (address_byte & 0b00000111));

            ip_progress += 2;
            break;
        }
        default: {
            return false;
        }
    }

    if (destination == 0) {
        uint16_t segment = ((destination_virtual_addr + 0x10 * survivor->DS) & 0xF0000) >> 16;
        if (segment != 0 && segment != survivor->stack_id && segment != shared_memory) {return false;}

        destination = (uint8_t*)&((char *) memory)[(uint32_t) destination_virtual_addr + survivor->DS * 0x10];
    }

    general_add(survivor, 0, address, 0, destination, 0);

    survivor->IP += ip_progress;
    return true;
}

// Might be a difference between our implementation and official implementation here.
// In the official implementation the shared memory loops back to stack, here it loops back to the shared memory.
// in the official implementation the loopback works differently for 16-bit registers.
// Unlike official implementation, here there is an exploit that allows survivors to access the first byte of
// another survivor's private section.
bool op_01(Survivor* survivor, uint16_t shared_memory) // ADD [X], reg16
{

    uint8_t address_byte = memory[0].values[survivor->IP+1];
    uint16_t pos = survivor->IP+2;

    uint8_t* significant_address,* insignificant_address;

    insignificant_address = (uint8_t*)reg16_decoder(survivor, (address_byte & 0b00111000) >> 3);
    significant_address = insignificant_address + 1;

    int ip_progress = 0;

    uint16_t destination_virtual_addr = 0;
    uint8_t* significant_destination = 0;
    uint8_t* insignificant_destination;

    switch (address_byte >> 6) {
        case 0b00: {
            destination_virtual_addr = address_decoder_mode00(survivor, address_byte & 0b00000111, pos);

            ip_progress += 2;
            break;
        }
        case 0b01: {
            destination_virtual_addr = address_decoder_mode01(survivor, address_byte & 0b00000111, pos);

            ip_progress += 3;
            break;
        }
        case 0b10: {
            destination_virtual_addr = address_decoder_mode10(survivor, address_byte & 0b00000111, pos);

            ip_progress += 4;
            break;
        }
        case 0b11: {
            significant_destination = (uint8_t*)reg16_decoder(survivor, (address_byte & 0b00000111));
            insignificant_destination = significant_destination + 1;

            ip_progress += 2;
            break;
        }
        default: {
            return false;
        }
    }



    if (significant_destination == 0) {
        uint16_t segment = ((destination_virtual_addr + 0x10 * survivor->DS) & 0xF0000) >> 16;
        if (segment != 0 && segment != survivor->stack_id && segment != shared_memory) {return false;}

        significant_destination = (uint8_t*)&((char *) memory)[(uint32_t) destination_virtual_addr + survivor->DS * 0x10];
        insignificant_destination = (uint8_t*)&((char *) memory)[(uint32_t) ((destination_virtual_addr+1)&0xFFFF) + survivor->DS * 0x10];
    }


    general_add(survivor, 1, significant_address, insignificant_address, significant_destination, insignificant_destination);

    survivor->IP += ip_progress;
    return true;
}

// might be a difference between our implementation and official implementation here.
// in the official implementation there is an exception when we push or pop from the end of the stack,
// here we just loop to the beginning of the segment.
bool op_06(Survivor* survivor, uint16_t shared_memory) {
    debug_print_statement
    survivor->SP -= 2;
    uint16_t destination = survivor->SP;

    uint16_t segment = ((destination+0x10*survivor->SS) & 0xF0000) >> 16;
    if (segment != 0 && segment != survivor->stack_id && segment != shared_memory) {return false;}

    ((char*)memory)[(uint32_t) destination + 0x10*survivor->SS] = (survivor->ES & 0xFF00) >> 8;

    destination--;
    ((char*)memory)[(uint32_t) destination + 0x10*survivor->SS] += survivor->ES & 0xFF;

    survivor->IP += 1;

    return true;

}

bool op_07(Survivor* survivor, uint16_t shared_memory) {
    debug_print_statement
    uint16_t address = survivor->SP;

    uint16_t segment = ((address+0x10*survivor->SS) & 0xF0000) >> 16;
    if (segment != 0 && segment != survivor->stack_id && segment != shared_memory) {return false;}

    survivor->ES = ((char*)memory)[(uint32_t) address + 0x10*survivor->SS] << 8;

    address--;
    survivor->ES |= ((char*)memory)[(uint32_t) address + 0x10*survivor->SS];

    survivor->SP += 2;

    survivor->IP += 1;

    return true;

}

void test_func(Survivor* survivor) {survivor->ES += 0x0110;}
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
    destination_virtual_addr = address_decoder_mode01(survivor, address_byte & 0b00000111, pos);

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

    uint16_t segment = ((destination_virtual_addr + 0x10 * survivor->DS) & 0xF0000) >> 16;
    if (segment != 0 && segment != survivor->stack_id && segment != shared_memory) {return false;}

    if (destination == 0) {
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
    uint16_t* address;
    uint16_t pos = survivor->IP + 2;

    address = reg16_decoder(survivor, (address_byte & 0b00111000) >> 3);

    switch(address_byte >> 6) {
        case 0b00: {
            uint16_t destination; // it is important that destination is 16 bit!
            destination = address_decoder_mode00(survivor, address_byte & 0b00000111, pos);

            uint16_t segment = ((destination+0x10*survivor->DS) & 0xF0000) >> 16;
            if (segment != 0 && segment != survivor->stack_id && segment != shared_memory) {return false;}

            ((char*)memory)[(uint32_t) destination + survivor->DS*0x10] += (*address) & 0xFF;

            destination++;
            ((char*)memory)[(uint32_t) destination + survivor->DS*0x10] += ((*address) & 0xFF00) >> 8;

            survivor->IP += 2;
            return true;
        }
        case 0b01: {
            uint16_t destination; // it is important that destination is 16 bit!
            destination = address_decoder_mode01(survivor, address_byte & 0b00000111, pos);

            uint16_t segment = ((destination+0x10*survivor->DS) & 0xF0000) >> 16;
            if (segment != 0 && segment != survivor->stack_id && segment != shared_memory) {return false;}

            ((char*)memory)[(uint32_t) destination + survivor->DS*0x10] += (*address) & 0xFF;

            destination++;
            ((char*)memory)[(uint32_t) destination + survivor->DS*0x10] += ((*address) & 0xFF00) >> 8;

            survivor->IP += 3;
            return true;
        }
        case 0b10: {
            uint16_t destination; // it is important that destination is 16 bit!
            destination = address_decoder_mode10(survivor, address_byte & 0b00000111, pos);

            uint16_t segment = ((destination+0x10*survivor->DS) & 0xF0000) >> 16;
            if (segment != 0 && segment != survivor->stack_id && segment != shared_memory) {return false;}

            ((char*)memory)[(uint32_t) destination + survivor->DS*0x10] += (*address) & 0xFF;

            destination++;
            ((char*)memory)[(uint32_t) destination + survivor->DS*0x10] += ((*address) & 0xFF00) >> 8;

            survivor->IP += 4;
            return true;
        }
        case 0b11: {
            uint16_t *destination;
            destination = reg16_decoder(survivor, address_byte & 0b00000111);
            *destination += *address;

            survivor->IP += 2;
            return true;
        }
    }
    return false;
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
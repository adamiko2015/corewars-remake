#include "structs_libraries_and_macros.h"
#include "globals.h"
#include "opcode_helper_functions.h"

bool op_00(Survivor* survivor, uint16_t shared_memory) // ADD [X], reg8
{
    uint8_t address_byte = memory[0].values[survivor->IP+1];
    uint16_t pos = survivor->IP+2;
    uint8_t* address;
    address = reg8_decoder(survivor, (address_byte & 0b00111000) >> 3);
    switch (address_byte >> 6) {
        case 0b00: {
            uint16_t destination;
            destination = address_decoder_mode00(survivor, address_byte & 0b00000111, pos);
            memory[0].values[destination] += *address;
            survivor->IP += 2;
            return true;
        }
        case 0b01: {
            uint16_t destination;
            destination = address_decoder_mode01(survivor, address_byte & 0b00000111, pos);
            memory[0].values[destination] += *address;
            survivor->IP += 3;
            return true;
        }
        case 0b10: {
            uint16_t destination;
            destination = address_decoder_mode10(survivor, address_byte & 0b00000111, pos);
            memory[0].values[destination] += *address;
            survivor->IP += 4;
            return true;
        }
        case 0b11: {
            uint8_t* destination;
            destination = reg8_decoder(survivor, (address_byte & 0b00000111));
            *destination += *address;
            survivor->IP += 2;
            return true;
        }
    }
    return false;
}

bool op_01(Survivor* survivor, uint16_t shared_memory) // ADD [X], reg16
{
    uint8_t address_byte = memory[0].values[survivor->IP+1];
    uint16_t* address;
    uint16_t pos = survivor->IP + 2;

    address = reg16_decoder(survivor, (address_byte & 0b00111000) >> 3);

    switch(address_byte >> 6) {
        case 0b00: {
            uint16_t destination;
            destination = address_decoder_mode00(survivor, address_byte & 0b00000111, pos);
            memory[0].values[destination] += *(uint8_t*)address;
            memory[0].values[(destination+1)%0x10000] += *(((uint8_t*)address)+1);
            survivor->IP += 2;
            return true;
        }
        case 0b01: {
            uint16_t destination;
            destination = address_decoder_mode01(survivor, address_byte & 0b00000111, pos);
            memory[0].values[destination] += *(uint8_t*)address;
            memory[0].values[(destination+1)%0x10000] += *(((uint8_t*)address)+1);
            survivor->IP += 3;
            return true;
        }
        case 0b10: {
            uint16_t destination;
            destination = address_decoder_mode10(survivor, address_byte & 0b00000111, pos);
            memory[0].values[destination] += *(uint8_t*)address;
            memory[0].values[(destination+1)%0x10000] += *(((uint8_t*)address)+1);
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
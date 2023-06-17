#include "structs_libraries_and_macros.h"
#include "globals.h"
#include "opcode_macros.h"

bool op_00(Survivor* survivor, uint16_t shared_memory) // ADD [X], reg8
{
    uint8_t address_byte = memory[0].values[survivor->IP+1];
    uint16_t pos = survivor->IP+2;
    uint8_t* address;
    reg8_decoder(address, (address_byte & 0b00111000) >> 3)
    switch (address_byte >> 6) {
        case 0b00:
            uint16_t destination;
            address_decoder_mode00(destination, address_byte & 0b00000111, pos)
            memory[0].values[destination] += *address;
            survivor->IP += 2;
            return true;
        case 0b01:
            uint16_t destination;
            address_decoder_mode00(destination, address_byte & 0b00000111, pos)
            memory[0].values[destination] += *address;
            survivor->IP += 3;
            return true;
        case 0b10:
            uint16_t destination;
            address_decoder_mode00(destination, address_byte & 0b00000111, pos)
            memory[0].values[destination] += *address;
            survivor->IP += 4;
            return true;
        case 0b11:
            uint8_t* destination;
            address_decoder_mode00(destination, address_byte & 0b00000111, pos)
            *destination += *address;
            survivor->IP += 2;
            return true;
    }
}

bool op_01(Survivor* survivor, uint16_t shared_memory) // ADD [X], reg16
{
    uint8_t address_byte = memory[0].values[survivor->IP+1];
    uint16_t* address;
    uint16_t pos = survivor->IP + 2;
    reg16_decoder(address, (address_byte & 0b00111000) >> 3)

    switch(address_byte >> 6) {
        case 0b00:
            uint16_t destination;
            address_decoder_mode00(destination, address_byte & 0b00000111, pos)
            memory[0].values[destination] += *address;
            survivor->IP += 2;
            return true;
        case 0b01:
            uint16_t destination;
            address_decoder_mode01(destination, address_byte & 0b00000111, pos)
            memory[0].values[destination] += *address;
            survivor->IP += 3;
            return true;
        case 0b10:
            uint16_t destination;
            address_decoder_mode10(destination, address_byte & 0b00000111, pos)
            memory[0].values[destination] += *address;
            survivor->IP += 4;
            return true;
        case 0b11:
            uint16_t* destination;
            reg16_decoder(destination,address_byte & 0b00000111)
            *destination += *address;
            survivor->IP += 2;
            return true;
    }
}
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

            uint16_t segment = (survivor->DS && 0x1000) >> 12;
            if (segment != 0 && segment != survivor->stack_id && segment != shared_memory) {return false;}

            ((char*)memory)[(uint32_t) destination + survivor->DS*0x10] += *address;

            survivor->IP += 2;
            return true;
        }
        case 0b01: {
            uint16_t destination;
            destination = address_decoder_mode01(survivor, address_byte & 0b00000111, pos);

            uint16_t segment = (survivor->DS && 0x1000) >> 12;
            if (segment != 0 && segment != survivor->stack_id && segment != shared_memory) {return false;}

            ((char*)memory)[(uint32_t) destination + survivor->DS*0x10] += *address;

            survivor->IP += 3;
            return true;
        }
        case 0b10: {
            uint16_t destination;
            destination = address_decoder_mode10(survivor, address_byte & 0b00000111, pos);

            uint16_t segment = (survivor->DS && 0x1000) >> 12;
            if (segment != 0 && segment != survivor->stack_id && segment != shared_memory) {return false;}

            ((char*)memory)[(uint32_t) destination + survivor->DS*0x10] += *address;

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

// might be a difference between our implementation and official implementation here.
// in the official implementation the shared memory loops back to stack, here it loops back to the shared memory.
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

            uint16_t segment = (survivor->DS & 0x1000) >> 12;
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

            uint16_t segment = (survivor->DS && 0x1000) >> 12;
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

            uint16_t segment = (survivor->DS && 0x1000) >> 12;
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
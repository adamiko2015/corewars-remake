#include "structs_libraries_and_macros.h"
#include "globals.h"
#include "opcode_helper_functions.h"

bool op_00(Survivor survivor[static 1], uint16_t shared_memory) // ADD [X], reg8
{
    uint8_t address_byte = memory[0].values[sregs.IP+1];
    uint16_t pos = sregs.IP+2;
    uint8_t* address;
    address = reg8_decoder(survivor, (address_byte & 0b00111000) >> 3);
    uint8_t ip_progress = 0;

    uint16_t destination_virtual_addr = 0;
    uint16_t segment_register_virtual_addr = 0;
    uint8_t* destination = 0;

    if (!get_virtual_address(&ip_progress, &destination, &destination_virtual_addr, survivor, &address_byte, pos, &segment_register_virtual_addr)) {return false;}

    if (destination == 0) {
        uint16_t segment = ((destination_virtual_addr + 0x10 * segment_register_virtual_addr) & 0xF0000) >> 16;
        if (segment != 0 && segment != survivor->stack_id && segment != shared_memory) {return false;}

       destination = (uint8_t*)&((char *) memory)[(uint32_t) destination_virtual_addr + segment_register_virtual_addr * 0x10];
    }

    general_add(survivor, 0, address, 0, destination, 0);

    sregs.IP += ip_progress;
    return true;
}

// Might be a difference between our implementation and official implementation here.
// In the official implementation the shared memory loops back to stack, here it loops back to the shared memory.
// in the official implementation the loopback works differently for 16-bit registers.
// Unlike official implementation, here there is an exploit that allows survivors to access the first byte of
// another survivor's private section.
bool op_01(Survivor survivor[static 1], uint16_t shared_memory) // ADD [X], reg16
{

    uint8_t address_byte = memory[0].values[sregs.IP+1];
    uint16_t pos = sregs.IP+2;

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
        uint16_t segment = ((destination_virtual_addr + 0x10 * segment_register_virtual_addr) & 0xF0000) >> 16;
        if (segment != 0 && segment != survivor->stack_id && segment != shared_memory) {return false;}

        insignificant_destination = (uint8_t*)&((char *) memory)[(uint32_t) destination_virtual_addr + segment_register_virtual_addr * 0x10];
        significant_destination = (uint8_t*)&((char *) memory)[(uint32_t) ((destination_virtual_addr+1)&0xFFFF) + segment_register_virtual_addr * 0x10];
    }
    else {
        significant_destination = insignificant_destination + 1;
    }


    general_add(survivor, 1, significant_address, insignificant_address, significant_destination, insignificant_destination);

    sregs.IP += ip_progress;
    return true;
}

bool op_02(Survivor survivor[static 1], uint16_t shared_memory) // ADD reg8, [X]
{
    uint8_t address_byte = memory[0].values[sregs.IP+1];
    uint16_t pos = sregs.IP+2;
    uint8_t* destination;
    destination = reg8_decoder(survivor, (address_byte & 0b00111000) >> 3);
    uint8_t ip_progress = 0;

    uint16_t address_virtual_addr = 0;
    uint16_t segment_register_virtual_addr = 0;
    uint8_t* address = 0;

    if (!get_virtual_address(&ip_progress, &destination, &address_virtual_addr, survivor, &address_byte, pos, &segment_register_virtual_addr)) {return false;}

    if (address == 0) {
        uint16_t segment = ((address_virtual_addr + 0x10 * segment_register_virtual_addr) & 0xF0000) >> 16;
        if (segment != 0 && segment != survivor->stack_id && segment != shared_memory) {return false;}

        address = (uint8_t*)&((char *) memory)[(uint32_t) address_virtual_addr + segment_register_virtual_addr * 0x10];
    }

    general_add(survivor, 0, address, 0, destination, 0);

    sregs.IP += ip_progress;
    return true;
}

// Might be a difference between our implementation and official implementation here.
// In the official implementation the shared memory loops back to stack, here it loops back to the shared memory.
// in the official implementation the loopback works differently for 16-bit registers.
// Unlike official implementation, here there is an exploit that allows survivors to access the first byte of
// another survivor's private section.
bool op_03(Survivor survivor[static 1], uint16_t shared_memory) // ADD reg16, [X]
{

    uint8_t address_byte = memory[0].values[sregs.IP+1];
    uint16_t pos = sregs.IP+2;

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
        uint16_t segment = ((address_virtual_addr + 0x10 * segment_register_virtual_addr) & 0xF0000) >> 16;
        if (segment != 0 && segment != survivor->stack_id && segment != shared_memory) {return false;}

        insignificant_address = (uint8_t*)&((char *) memory)[(uint32_t) address_virtual_addr + segment_register_virtual_addr * 0x10];
        significant_address = (uint8_t*)&((char *) memory)[(uint32_t) ((address_virtual_addr+1)&0xFFFF) + segment_register_virtual_addr * 0x10];
    }
    else {
        significant_destination = insignificant_destination - 1;
    }


    general_add(survivor, 1, significant_address, insignificant_address, significant_destination, insignificant_destination);

    sregs.IP += ip_progress;
    return true;
}


// might be a difference between our implementation and official implementation here.
// in the official implementation there is an exception when we push or pop from the end of the stack,
// here we just loop to the beginning of the segment.
bool op_06(Survivor survivor[static 1], uint16_t shared_memory) // Push ES
{
    return general_push(survivor, shared_memory, &sregs.ES);
}

bool op_07(Survivor survivor[static 1], uint16_t shared_memory) // Pop ES
{
    return general_pop(survivor, shared_memory, &sregs.ES);
}

void test_func(Survivor* survivor) {sregs.ES += 0x0110;}
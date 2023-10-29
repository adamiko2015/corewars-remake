#include "structs_libraries_and_macros.h"
#include "globals.h"
#include "opcode_helper_functions.h"
#include "opcode_flag_helper_functions.h"


bool general_binary_operation(Survivor survivor[static 1], uint16_t shared_memory) // OP *, *
{
    uint8_t opcode = memory[0].values[sregs.IP];
    operation_ptr operation = operators[(opcode & 0b11111000) >> 3];
    op_generalizer generalizer = general_ops[opcode & 0b111];

    return generalizer(survivor, shared_memory, operation);
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

bool op_0E(Survivor survivor[static 1], uint16_t shared_memory) // Push CS
{
    return general_push(survivor, shared_memory, &sregs.CS);
}

bool op_16(Survivor survivor[static 1], uint16_t shared_memory) // Push SS
{
    return general_push(survivor, shared_memory, &sregs.SS);
}

bool op_17(Survivor survivor[static 1], uint16_t shared_memory) // Pop SS
{
    return general_pop(survivor, shared_memory, &sregs.SS);
}

bool op_1E(Survivor survivor[static 1], uint16_t shared_memory) // Push DS
{
    return general_push(survivor, shared_memory, &sregs.DS);
}

bool op_1F(Survivor survivor[static 1], uint16_t shared_memory) // Pop DS
{
    return general_pop(survivor, shared_memory, &sregs.DS);
}

bool general_inc(Survivor survivor[static 1], uint16_t shared_memory) // INC reg16
{
    debug_print_statement

    uint8_t reg_byte = memory[0].values[sregs.IP];
    uint16_t* reg = reg16_decoder(survivor, reg_byte & 0b00000111);

    uint16_t flags_to_update = flags_16_bit_add(*reg, 1);
    update_specific_flags(survivor, sregs.Flags, flags_to_update, 0x08d5);

    (*reg)++;

    sregs.IP++;

    return true;
}

bool general_dec(Survivor survivor[static 1], uint16_t shared_memory) // DEC reg16
{
    debug_print_statement

    uint8_t reg_byte = memory[0].values[sregs.IP];
    uint16_t* reg = reg16_decoder(survivor, reg_byte & 0b00000111);

    uint16_t flags_to_update = flags_16_bit_sub(*reg, 1);
    update_specific_flags(survivor, sregs.Flags, flags_to_update, 0x08d5);

    (*reg)--;

    sregs.IP++;

    return true;
}

bool general_push_opcode(Survivor survivor[static 1], uint16_t shared_memory) // PUSH reg16
{
    debug_print_statement

    uint8_t reg_byte = memory[0].values[sregs.IP];
    uint16_t* reg = reg16_decoder(survivor, reg_byte & 0b00000111);

    return general_push(survivor, shared_memory, reg);
}

bool general_pop_opcode(Survivor survivor[static 1], uint16_t shared_memory) // POP reg16
{
    debug_print_statement

    uint8_t reg_byte = memory[0].values[sregs.IP];
    uint16_t* reg = reg16_decoder(survivor, reg_byte & 0b00000111);

    return general_pop(survivor, shared_memory, reg);
}
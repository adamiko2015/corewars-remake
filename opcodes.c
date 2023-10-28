#include "structs_libraries_and_macros.h"
#include "globals.h"
#include "opcode_helper_functions.h"


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


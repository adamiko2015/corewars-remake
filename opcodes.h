#ifndef COREWARSINTERPRETER_OPCODES_H
#define COREWARSINTERPRETER_OPCODES_H

bool general_binary_operation(Survivor survivor[static 1], uint16_t shared_memory);

bool op_06(Survivor survivor[static 1], uint16_t shared_memory);
bool op_07(Survivor survivor[static 1], uint16_t shared_memory);

bool op_0E(Survivor survivor[static 1], uint16_t shared_memory);

bool op_16(Survivor survivor[static 1], uint16_t shared_memory);
bool op_17(Survivor survivor[static 1], uint16_t shared_memory);

bool op_1E(Survivor survivor[static 1], uint16_t shared_memory);
bool op_1F(Survivor survivor[static 1], uint16_t shared_memory);

bool general_inc(Survivor survivor[static 1], uint16_t shared_memory);
bool general_dec(Survivor survivor[static 1], uint16_t shared_memory);

bool general_push_opcode(Survivor survivor[static 1], uint16_t shared_memory);
bool general_pop_opcode(Survivor survivor[static 1], uint16_t shared_memory);

#endif
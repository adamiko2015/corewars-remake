#ifndef COREWARSINTERPRETER_OPCODES_H
#define COREWARSINTERPRETER_OPCODES_H

bool general_binary_operation(Survivor survivor[static 1], uint16_t shared_memory);

bool op_00(Survivor survivor[static 1], uint16_t shared_memory);
bool op_01(Survivor survivor[static 1], uint16_t shared_memory);
bool op_02(Survivor survivor[static 1], uint16_t shared_memory);
bool op_03(Survivor survivor[static 1], uint16_t shared_memory);

bool op_06(Survivor survivor[static 1], uint16_t shared_memory);
bool op_07(Survivor survivor[static 1], uint16_t shared_memory);

#endif
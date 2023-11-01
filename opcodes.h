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

bool op_70(Survivor survivor[static 1], uint16_t shared_memory);
bool op_71(Survivor survivor[static 1], uint16_t shared_memory);
bool op_72(Survivor survivor[static 1], uint16_t shared_memory);
bool op_73(Survivor survivor[static 1], uint16_t shared_memory);
bool op_74(Survivor survivor[static 1], uint16_t shared_memory);
bool op_75(Survivor survivor[static 1], uint16_t shared_memory);
bool op_76(Survivor survivor[static 1], uint16_t shared_memory);
bool op_77(Survivor survivor[static 1], uint16_t shared_memory);
bool op_78(Survivor survivor[static 1], uint16_t shared_memory);
bool op_79(Survivor survivor[static 1], uint16_t shared_memory);
bool op_7A(Survivor survivor[static 1], uint16_t shared_memory);
bool op_7B(Survivor survivor[static 1], uint16_t shared_memory);
bool op_7C(Survivor survivor[static 1], uint16_t shared_memory);
bool op_7D(Survivor survivor[static 1], uint16_t shared_memory);
bool op_7E(Survivor survivor[static 1], uint16_t shared_memory);
bool op_7F(Survivor survivor[static 1], uint16_t shared_memory);

bool op_80(Survivor survivor[static 1], uint16_t shared_memory);
bool op_81(Survivor survivor[static 1], uint16_t shared_memory);
bool op_82(Survivor survivor[static 1], uint16_t shared_memory);
bool op_83(Survivor survivor[static 1], uint16_t shared_memory);

bool op_84(Survivor survivor[static 1], uint16_t shared_memory);
bool op_85(Survivor survivor[static 1], uint16_t shared_memory);

bool general_inc(Survivor survivor[static 1], uint16_t shared_memory);
bool general_dec(Survivor survivor[static 1], uint16_t shared_memory);

bool general_push_opcode(Survivor survivor[static 1], uint16_t shared_memory);
bool general_pop_opcode(Survivor survivor[static 1], uint16_t shared_memory);

#endif
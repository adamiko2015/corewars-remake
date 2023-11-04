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

bool op_86(Survivor survivor[static 1], uint16_t shared_memory);
bool op_87(Survivor survivor[static 1], uint16_t shared_memory);

bool general_mov_op(Survivor survivor[static 1], uint16_t shared_memory);

bool op_8C(Survivor survivor[static 1], uint16_t shared_memory);
bool op_8D(Survivor survivor[static 1], uint16_t shared_memory);
bool op_8E(Survivor survivor[static 1], uint16_t shared_memory);
bool op_8F(Survivor survivor[static 1], uint16_t shared_memory);

bool general_xchg_op(Survivor survivor[static 1], uint16_t shared_memory);

bool op_98(Survivor survivor[static 1], uint16_t shared_memory);
bool op_99(Survivor survivor[static 1], uint16_t shared_memory);
bool op_9A(Survivor survivor[static 1], uint16_t shared_memory);
bool op_9B(Survivor survivor[static 1], uint16_t shared_memory);
bool op_9C(Survivor survivor[static 1], uint16_t shared_memory);
bool op_9D(Survivor survivor[static 1], uint16_t shared_memory);
bool op_9E(Survivor survivor[static 1], uint16_t shared_memory);
bool op_9F(Survivor survivor[static 1], uint16_t shared_memory);
bool op_A0(Survivor survivor[static 1], uint16_t shared_memory);
bool op_A1(Survivor survivor[static 1], uint16_t shared_memory);
bool op_A2(Survivor survivor[static 1], uint16_t shared_memory);
bool op_A3(Survivor survivor[static 1], uint16_t shared_memory);
bool op_A4(Survivor survivor[static 1], uint16_t shared_memory);
bool op_A5(Survivor survivor[static 1], uint16_t shared_memory);
bool op_A6(Survivor survivor[static 1], uint16_t shared_memory);
bool op_A7(Survivor survivor[static 1], uint16_t shared_memory);
bool op_A8(Survivor survivor[static 1], uint16_t shared_memory);
bool op_A9(Survivor survivor[static 1], uint16_t shared_memory);
bool op_AA(Survivor survivor[static 1], uint16_t shared_memory);
bool op_AB(Survivor survivor[static 1], uint16_t shared_memory);
bool op_AC(Survivor survivor[static 1], uint16_t shared_memory);
bool op_AD(Survivor survivor[static 1], uint16_t shared_memory);
bool op_AE(Survivor survivor[static 1], uint16_t shared_memory);
bool op_AF(Survivor survivor[static 1], uint16_t shared_memory);

bool op_B0to7(Survivor survivor[static 1], uint16_t shared_memory);
bool op_B8toF(Survivor survivor[static 1], uint16_t shared_memory);

bool op_C2(Survivor survivor[static 1], uint16_t shared_memory);
bool op_C3(Survivor survivor[static 1], uint16_t shared_memory);
bool op_C4(Survivor survivor[static 1], uint16_t shared_memory);
bool op_C5(Survivor survivor[static 1], uint16_t shared_memory);
bool op_C6(Survivor survivor[static 1], uint16_t shared_memory);
bool op_C7(Survivor survivor[static 1], uint16_t shared_memory);

bool op_CA(Survivor survivor[static 1], uint16_t shared_memory);
bool op_CB(Survivor survivor[static 1], uint16_t shared_memory);

bool op_CD(Survivor survivor[static 1], uint16_t shared_memory);

bool op_CF(Survivor survivor[static 1], uint16_t shared_memory);

bool op_D0(Survivor survivor[static 1], uint16_t shared_memory);
bool op_D1(Survivor survivor[static 1], uint16_t shared_memory);
bool op_D2(Survivor survivor[static 1], uint16_t shared_memory);
bool op_D3(Survivor survivor[static 1], uint16_t shared_memory);

bool op_D7(Survivor survivor[static 1], uint16_t shared_memory);

bool op_E0(Survivor survivor[static 1], uint16_t shared_memory);
bool op_E1(Survivor survivor[static 1], uint16_t shared_memory);
bool op_E2(Survivor survivor[static 1], uint16_t shared_memory);
bool op_E3(Survivor survivor[static 1], uint16_t shared_memory);

bool op_E8(Survivor survivor[static 1], uint16_t shared_memory);
bool op_E9(Survivor survivor[static 1], uint16_t shared_memory);
bool op_EA(Survivor survivor[static 1], uint16_t shared_memory);
bool op_EB(Survivor survivor[static 1], uint16_t shared_memory);


bool op_F2(Survivor survivor[static 1], uint16_t shared_memory);
bool op_F3(Survivor survivor[static 1], uint16_t shared_memory);

bool op_F5(Survivor survivor[static 1], uint16_t shared_memory);

bool general_inc(Survivor survivor[static 1], uint16_t shared_memory);
bool general_dec(Survivor survivor[static 1], uint16_t shared_memory);

bool general_push_opcode(Survivor survivor[static 1], uint16_t shared_memory);
bool general_pop_opcode(Survivor survivor[static 1], uint16_t shared_memory);

#endif
#include "globals.h"
#include "structs_libraries_and_macros.h"

#ifndef COREWARSINTERPRETER_OPCODE_MACROS_H
#define COREWARSINTERPRETER_OPCODE_MACROS_H

bool get_virtual_address(uint8_t ip_progress[static 1], uint8_t* destination[static 1], uint16_t destination_virtual_addr[static 1],
                         Survivor survivor[static 1], uint8_t address_byte[static 1], uint16_t pos, uint16_t segment_register_virtual_addr[static 1]);

uint16_t address_decoder_mode00(Survivor survivor[static 1], uint8_t byte, uint16_t pos, uint16_t segment_register_virtual_addr[static 1], uint8_t ip_progress[static 1]);
uint16_t address_decoder_mode01(Survivor survivor[static 1], uint8_t byte, uint16_t segment_register_virtual_addr[static 1], uint16_t pos);
uint16_t address_decoder_mode10(Survivor survivor[static 1], uint8_t byte, uint16_t segment_register_virtual_addr[static 1], uint16_t pos);

uint8_t* reg8_decoder(Survivor survivor[static 1], uint8_t byte);
uint16_t* reg16_decoder(Survivor survivor[static 1], uint8_t byte);
uint16_t* seg_decoder(Survivor survivor[static 1], uint8_t byte);


void general_add(Survivor survivor[static 1], bool is_16_bit, uint8_t significant_from[static 1], uint8_t* insignificant_from,
                uint8_t significant_to[static 1], uint8_t* insignificant_to);

void general_or(Survivor survivor[static 1], bool is_16_bit, uint8_t significant_from[static 1], uint8_t* insignificant_from,
                uint8_t significant_to[static 1], uint8_t* insignificant_to);

void general_adc(Survivor survivor[static 1], bool is_16_bit, uint8_t significant_from[static 1], uint8_t* insignificant_from,
                uint8_t significant_to[static 1], uint8_t* insignificant_to);

void general_sbb(Survivor survivor[static 1], bool is_16_bit, uint8_t significant_from[static 1], uint8_t* insignificant_from,
                uint8_t significant_to[static 1], uint8_t* insignificant_to);

void general_and(Survivor survivor[static 1], bool is_16_bit, uint8_t significant_from[static 1], uint8_t* insignificant_from,
                uint8_t significant_to[static 1], uint8_t* insignificant_to);

void general_sub(Survivor survivor[static 1], bool is_16_bit, uint8_t significant_from[static 1], uint8_t* insignificant_from,
                uint8_t significant_to[static 1], uint8_t* insignificant_to);

void general_xor(Survivor survivor[static 1], bool is_16_bit, uint8_t significant_from[static 1], uint8_t* insignificant_from,
                uint8_t significant_to[static 1], uint8_t* insignificant_to);

void general_cmp(Survivor survivor[static 1], bool is_16_bit, uint8_t significant_from[static 1], uint8_t* insignificant_from,
                uint8_t significant_to[static 1], uint8_t* insignificant_to);

void general_mov(Survivor survivor[static 1], bool is_16_bit, uint8_t significant_from[static 1], uint8_t* insignificant_from,
                uint8_t significant_to[static 1], uint8_t* insignificant_to);

void general_test(Survivor survivor[static 1], bool is_16_bit, uint8_t significant_from[static 1], uint8_t* insignificant_from,
                uint8_t significant_to[static 1], uint8_t* insignificant_to);

void general_rol(Survivor survivor[static 1], bool is_16_bit, uint8_t significant_from[static 1], uint8_t* insignificant_from,
                  uint8_t significant_to[static 1], uint8_t* insignificant_to);
void general_ror(Survivor survivor[static 1], bool is_16_bit, uint8_t significant_from[static 1], uint8_t* insignificant_from,
                  uint8_t significant_to[static 1], uint8_t* insignificant_to);
void general_rcl(Survivor survivor[static 1], bool is_16_bit, uint8_t significant_from[static 1], uint8_t* insignificant_from,
                  uint8_t significant_to[static 1], uint8_t* insignificant_to);
void general_rcr(Survivor survivor[static 1], bool is_16_bit, uint8_t significant_from[static 1], uint8_t* insignificant_from,
                  uint8_t significant_to[static 1], uint8_t* insignificant_to);
void general_shl(Survivor survivor[static 1], bool is_16_bit, uint8_t significant_from[static 1], uint8_t* insignificant_from,
                  uint8_t significant_to[static 1], uint8_t* insignificant_to);
void general_shr(Survivor survivor[static 1], bool is_16_bit, uint8_t significant_from[static 1], uint8_t* insignificant_from,
                  uint8_t significant_to[static 1], uint8_t* insignificant_to);
void general_sar(Survivor survivor[static 1], bool is_16_bit, uint8_t significant_from[static 1], uint8_t* insignificant_from,
                  uint8_t significant_to[static 1], uint8_t* insignificant_to);

void general_xchg(Survivor survivor[static 1], bool is_16_bit, uint8_t significant_from[static 1], uint8_t* insignificant_from,
                  uint8_t significant_to[static 1], uint8_t* insignificant_to);

void general_mul(Survivor survivor[static 1], bool is_16_bit, uint8_t significant_from[static 1], uint8_t* insignificant_from);

void general_div(Survivor survivor[static 1], bool is_16_bit, uint8_t significant_from[static 1], uint8_t* insignificant_from);


void update_specific_flags(Survivor survivor[static 1], uint16_t og_flags, uint16_t flags_after_change, uint16_t mask);

bool general_push(Survivor survivor[static 1], uint16_t shared_memory, uint16_t reg[static 1]);
bool general_pop(Survivor survivor[static 1], uint16_t shared_memory, uint16_t reg[static 1]);

bool general_op_0(Survivor survivor[static 1], uint16_t shared_memory, operation_ptr general_op);
bool general_op_1(Survivor survivor[static 1], uint16_t shared_memory, operation_ptr general_op);
bool general_op_2(Survivor survivor[static 1], uint16_t shared_memory, operation_ptr general_op);
bool general_op_3(Survivor survivor[static 1], uint16_t shared_memory, operation_ptr general_op);
bool general_op_4(Survivor survivor[static 1], uint16_t shared_memory, operation_ptr general_op);
bool general_op_5(Survivor survivor[static 1], uint16_t shared_memory, operation_ptr general_op);
bool general_op_6(Survivor survivor[static 1], uint16_t shared_memory, operation_ptr general_op);
bool general_op_7(Survivor survivor[static 1], uint16_t shared_memory, operation_ptr general_op);
bool general_op_8(Survivor survivor[static 1], uint16_t shared_memory, operation_ptr general_op);
bool general_op_9(Survivor survivor[static 1], uint16_t shared_memory, operation_ptr general_op);
bool general_op_10(Survivor survivor[static 1], uint16_t shared_memory, operation_ptr general_op);
bool general_op_11(Survivor survivor[static 1], uint16_t shared_memory, operation_ptr general_op);
bool general_op_12(Survivor survivor[static 1], uint16_t shared_memory, operation_ptr general_op);
bool general_op_13(Survivor survivor[static 1], uint16_t shared_memory, operation_ptr general_op);
bool general_op_14(Survivor survivor[static 1], uint16_t shared_memory, operation_ptr general_op);
bool general_op_15(Survivor survivor[static 1], uint16_t shared_memory, operation_ptr general_op);
bool general_op_16(Survivor survivor[static 1], uint16_t shared_memory, operation_ptr general_op);

void general_jmp_near_2B_opcode(Survivor survivor[static 1]);

#endif //COREWARSINTERPRETER_OPCODE_MACROS_H

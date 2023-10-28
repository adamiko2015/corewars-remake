//
// Created by Guy on 10/8/2023.
//

#ifndef COREWARSINTERPRETER_OPCODE_FLAG_HELPER_FUNCTIONS_H
#define COREWARSINTERPRETER_OPCODE_FLAG_HELPER_FUNCTIONS_H

uint16_t flags_8_bit_add(uint8_t num_a, uint8_t num_b);
uint16_t flags_16_bit_add(uint16_t num_a, uint16_t num_b);

uint16_t flags_8_bit_or(uint8_t num_a, uint8_t num_b);
uint16_t flags_16_bit_or(uint16_t num_a, uint16_t num_b);

uint16_t flags_8_bit_adc(uint8_t num_a, uint8_t num_b);
uint16_t flags_16_bit_adc(uint16_t num_a, uint16_t num_b);

uint16_t flags_8_bit_sbb(uint8_t num_a, uint8_t num_b);
uint16_t flags_16_bit_sbb(uint16_t num_a, uint16_t num_b);

uint16_t flags_8_bit_and(uint8_t num_a, uint8_t num_b);
uint16_t flags_16_bit_and(uint16_t num_a, uint16_t num_b);

uint16_t flags_8_bit_sub(uint8_t num_a, uint8_t num_b);
uint16_t flags_16_bit_sub(uint16_t num_a, uint16_t num_b);

uint16_t flags_8_bit_xor(uint8_t num_a, uint8_t num_b);
uint16_t flags_16_bit_xor(uint16_t num_a, uint16_t num_b);

uint16_t flags_8_bit_cmp(uint8_t num_a, uint8_t num_b);
uint16_t flags_16_bit_cmp(uint16_t num_a, uint16_t num_b);

uint16_t flags_8_bit_mov(uint8_t num_a, uint8_t num_b);
uint16_t flags_16_bit_mov(uint16_t num_a, uint16_t num_b);

uint16_t flags_8_bit_test(uint8_t num_a, uint8_t num_b);
uint16_t flags_16_bit_test(uint16_t num_a, uint16_t num_b);

#endif //COREWARSINTERPRETER_OPCODE_FLAG_HELPER_FUNCTIONS_H

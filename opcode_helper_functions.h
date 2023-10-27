#include "globals.h"
#include "structs_libraries_and_macros.h"

#ifndef COREWARSINTERPRETER_OPCODE_MACROS_H
#define COREWARSINTERPRETER_OPCODE_MACROS_H

uint16_t address_decoder_mode00(Survivor survivor[static 1], uint8_t byte, uint16_t pos, uint8_t ip_progress[static 1]);
uint16_t address_decoder_mode01(Survivor survivor[static 1], uint8_t byte, uint16_t pos);
uint16_t address_decoder_mode10(Survivor survivor[static 1], uint8_t byte, uint16_t pos);

uint8_t* reg8_decoder(Survivor survivor[static 1], uint8_t byte);
uint16_t* reg16_decoder(Survivor survivor[static 1], uint8_t byte);

void general_add(Survivor survivor[static 1], bool is_16_bit, uint8_t significant_from[static 1], uint8_t* insignificant_from,
                 uint8_t significant_to[static 1], uint8_t* insignificant_to);

void update_specific_flags(Survivor survivor[static 1], uint16_t og_flags, uint16_t flags_after_change, uint16_t mask);

bool general_push(Survivor survivor[static 1], uint16_t shared_memory, uint16_t reg[static 1]);
bool general_pop(Survivor survivor[static 1], uint16_t shared_memory, uint16_t reg[static 1]);

#endif //COREWARSINTERPRETER_OPCODE_MACROS_H

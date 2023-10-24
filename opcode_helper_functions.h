#include "globals.h"
#include "structs_libraries_and_macros.h"

#ifndef COREWARSINTERPRETER_OPCODE_MACROS_H
#define COREWARSINTERPRETER_OPCODE_MACROS_H

uint16_t address_decoder_mode00(Survivor* survivor, uint8_t byte, uint16_t pos);
uint16_t address_decoder_mode01(Survivor* survivor, uint8_t byte, uint16_t pos);
uint16_t address_decoder_mode10(Survivor* survivor, uint8_t byte, uint16_t pos);

uint8_t* reg8_decoder(Survivor* survivor, uint8_t byte);
uint16_t* reg16_decoder(Survivor* survivor, uint8_t byte);

void general_add(Survivor* survivor, bool is_16_bit, uint8_t* significant_from, uint8_t* insignificant_from,
                 uint8_t* significant_to, uint8_t* insignificant_to);

void update_specific_flags(Survivor* survivor, uint16_t og_flags, uint16_t flags_after_change, uint16_t mask);

bool general_push(Survivor* survivor, uint16_t shared_memory, uint16_t* reg);
bool general_pop(Survivor* survivor, uint16_t shared_memory, uint16_t* reg);

#endif //COREWARSINTERPRETER_OPCODE_MACROS_H

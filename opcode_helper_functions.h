#include "globals.h"
#include "structs_libraries_and_macros.h"

#ifndef COREWARSINTERPRETER_OPCODE_MACROS_H
#define COREWARSINTERPRETER_OPCODE_MACROS_H

uint16_t address_decoder_mode00(Survivor* survivor, uint8_t byte, uint16_t pos);
uint16_t address_decoder_mode01(Survivor* survivor, uint8_t byte, uint16_t pos);
uint16_t address_decoder_mode10(Survivor* survivor, uint8_t byte, uint16_t pos);

uint8_t* reg8_decoder(Survivor* survivor, uint8_t byte);
uint16_t* reg16_decoder(Survivor* survivor, uint8_t byte);

#endif //COREWARSINTERPRETER_OPCODE_MACROSS_H

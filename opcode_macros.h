#ifndef COREWARSINTERPRETER_OPCODE_MACROS_H
#define COREWARSINTERPRETER_OPCODE_MACROS_H

// assume byte is of the form 0b00000xxx
#define address_decoder_mode00(address, byte, pos) {
    switch(byte) {
        case 0b000:
            address = survivor->BX + survivor->SI;
            address %= 0x10000;
            break;
        case 0b001:
            address = survivor->BX + survivor->DI;
            address %= 0x10000;
            break;
        case 0b010:
            address = survivor->BP + survivor->SI;
            address %= 0x10000;
            break;
        case 0b011:
            address = survivor->BP + survivor->DI;
            address %= 0x10000;
            break;
        case 0b100:
            address = survivor->SI;
            break;
        case 0b101:
            address = survivor->DI;
            break;
        case 0b110:
            address = memory[0].values[pos+1] + memory[0].values[pos+2]*0x100;
            address %= 0x10000;
            break;
        case 0b111:
            address = survivor->BX;
            break;
    }
}

#define address_decoder_mode01(address, byte, pos) { // same as mod00, offset by next byte
    switch(byte) {
        case 0b000:
            address = survivor->BX + survivor->SI + memory[0].values[pos+1];
            address %= 0x10000;
            break;
        case 0b001:
            address = survivor->BX + survivor->DI + memory[0].values[pos+1];
            address %= 0x10000;
            break;
        case 0b010:
            address = survivor->BP + survivor->SI + memory[0].values[pos+1];
            address %= 0x10000;
            break;
        case 0b011:
            address = survivor->BP + survivor->DI + memory[0].values[pos+1];
            address %= 0x10000;
            break;
        case 0b100:
            address = survivor->SI + memory[0].values[pos+1];
            address %= 0x10000;
            break;
        case 0b101:
            address = survivor->DI + memory[0].values[pos+1];
            address %= 0x10000;
            break;
        case 0b110:
            address = survivor->BP + memory[0].values[pos+1];
            address %= 0x10000;
            break;
        case 0b111:
            address = survivor->BX + memory[0].values[pos+1];
            address %= 0x10000;
            break;
    }
}

#define address_decoder_mode10(address, byte, pos) {
    switch(byte) {
        case 0b000:
            address = survivor->BX + survivor->SI + memory[0].values[pos+1] + memory[0].values[pos+2]*0x100;
            address %= 0x10000;
            break;
        case 0b001:
            address = survivor->BX + survivor->DI + memory[0].values[pos+1] + memory[0].values[pos+2]*0x100;
            address %= 0x10000;
            break;
        case 0b010:
            address = survivor->BP + survivor->SI + memory[0].values[pos+1] + memory[0].values[pos+2]*0x100;
            address %= 0x10000;
            break;
        case 0b011:
            address = survivor->BP + survivor->DI + memory[0].values[pos+1] + memory[0].values[pos+2]*0x100;
            address %= 0x10000;
            break;
        case 0b100:
            address = survivor->SI + memory[0].values[pos+1] + memory[0].values[pos+2]*0x100;
            address %= 0x10000;
            break;
        case 0b101:
            address = survivor->DI + memory[0].values[pos+1] + memory[0].values[pos+2]*0x100;
            address %= 0x10000;
            break;
        case 0b110:
            address = survivor->BP + memory[0].values[pos+1] + memory[0].values[pos+2]*0x100;
            address %= 0x10000;
            break;
        case 0b111:
            address = survivor->BX + memory[0].values[pos+1] + memory[0].values[pos+2]*0x100;
            address %= 0x10000;
            break;
    }
}

// TODO: determine if little endian assumption is right
// TODO: find way of implementing without switch case?
#define reg8_decoder(reg, byte) {
    switch (byte) {
        case 0b000:
            reg = (uint8_t)&(survivor->AX);
            break;
        case 0b001:
            reg = (uint8_t)&(survivor->CX);
            break;
        case 0b010:
            reg = (uint8_t)&(survivor->DX);
            break;
        case 0b011:
            reg = (uint8_t)&(survivor->BX);
            break;
        case 0b100:
            reg = (uint8_t)&(survivor->AX) + 1;
            break;
        case 0b101:
            reg = (uint8_t)&(survivor->CX) + 1;
            break;
        case 0b110:
            reg = (uint8_t)&(survivor->DX) + 1;
            break;
        case 0b111:
            reg = (uint8_t)&(survivor->BX) + 1;
            break;
    }
}

#define reg16_decoder(reg, byte) {
    switch(byte) {
        case 0b000:
            reg = &(survivor->AX);
            break;
        case 0b001:
            reg = &(survivor->CX);
            break;
        case 0b010:
            reg = &(survivor->DX);
            break;
        case 0b011:
            reg = &(survivor->BX);
            break;
        case 0b100:
            reg = &(survivor->SP);
            break;
        case 0b101:
            reg = &(survivor->BP);
            break; 
        case 0b110:
            reg = &(survivor->SI);
            break;
        case 0b111:
            reg = &(survivor->DI);
            break;
    }
}

#endif
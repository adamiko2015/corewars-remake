#include "structs_libraries_and_macros.h"
#include "globals.h"
#include "opcode_helper_functions.h"


bool general_binary_operation(Survivor survivor[static 1], uint16_t shared_memory) // OP *, * (general_op0/1/2/3/4/5)
{
    uint8_t opcode = memory[0].values[sregs.IP + 0x10*sregs.CS];
    operation_ptr operation = operators[(opcode & 0b11111000) >> 3];
    op_generalizer generalizer = general_ops[opcode & 0b111];

    return generalizer(survivor, shared_memory, operation);
}

// might be a difference between our implementation and official implementation here.
// in the official implementation there is an exception when we push or pop from the end of the stack,
// here we just loop to the beginning of the segment.
bool op_06(Survivor survivor[static 1], uint16_t shared_memory) // Push ES
{
    return general_push(survivor, shared_memory, &sregs.ES);
}

bool op_07(Survivor survivor[static 1], uint16_t shared_memory) // Pop ES
{
    return general_pop(survivor, shared_memory, &sregs.ES);
}

bool op_0E(Survivor survivor[static 1], uint16_t shared_memory) // Push CS
{
    return general_push(survivor, shared_memory, &sregs.CS);
}

bool op_16(Survivor survivor[static 1], uint16_t shared_memory) // Push SS
{
    return general_push(survivor, shared_memory, &sregs.SS);
}

bool op_17(Survivor survivor[static 1], uint16_t shared_memory) // Pop SS
{
    return general_pop(survivor, shared_memory, &sregs.SS);
}

bool op_1E(Survivor survivor[static 1], uint16_t shared_memory) // Push DS
{
    return general_push(survivor, shared_memory, &sregs.DS);
}

bool op_1F(Survivor survivor[static 1], uint16_t shared_memory) // Pop DS
{
    return general_pop(survivor, shared_memory, &sregs.DS);
}

bool general_inc(Survivor survivor[static 1], uint16_t shared_memory) // INC reg16
{
    uint8_t reg_byte = memory[0].values[sregs.IP + 0x10*sregs.CS];
    uint16_t* reg = reg16_decoder(survivor, reg_byte & 0b00000111);

    uint_fast16_t flags_to_update;

    asm (   "inc %2\r\n\t"
            "mov %0, %2\r\n\t"
            "pushf\r\n\t"
            "pop %1"
            : "=r" (*reg), "=r" (flags_to_update)
            : "r" (*reg)
            );

    update_specific_flags(survivor, sregs.Flags, flags_to_update, 0x08d5);

    sregs.IP++;

    return true;
}

bool general_dec(Survivor survivor[static 1], uint16_t shared_memory) // DEC reg16
{
    uint8_t reg_byte = memory[0].values[sregs.IP + 0x10*sregs.CS];
    uint16_t* reg = reg16_decoder(survivor, reg_byte & 0b00000111);

    uint_fast16_t flags_to_update;

    asm (   "dec %2\r\n\t"
            "mov %0, %2\r\n\t"
            "pushf\r\n\t"
            "pop %1"
            : "=r" (*reg), "=r" (flags_to_update)
            : "r" (*reg)
            );

    update_specific_flags(survivor, sregs.Flags, flags_to_update, 0x08d5);

    sregs.IP++;

    return true;
}

bool general_push_opcode(Survivor survivor[static 1], uint16_t shared_memory) // PUSH reg16
{

    uint8_t reg_byte = memory[0].values[sregs.IP + 0x10*sregs.CS];
    uint16_t* reg = reg16_decoder(survivor, reg_byte & 0b00000111);

    return general_push(survivor, shared_memory, reg);
}

bool general_pop_opcode(Survivor survivor[static 1], uint16_t shared_memory) // POP reg16
{

    uint8_t reg_byte = memory[0].values[sregs.IP + 0x10*sregs.CS];
    uint16_t* reg = reg16_decoder(survivor, reg_byte & 0b00000111);

    return general_pop(survivor, shared_memory, reg);
}

bool op_70(Survivor survivor[static 1], uint16_t shared_memory) // JO
{

    bool condition = (sregs.Flags & 0x0800);
    if (condition) general_jmp_near_2B_opcode(survivor);
    else sregs.IP += 2;
    return true;
}

bool op_71(Survivor survivor[static 1], uint16_t shared_memory) // JNO
{

    bool condition = !(sregs.Flags & 0x0800);
    if (condition) general_jmp_near_2B_opcode(survivor);
    else sregs.IP += 2;
    return true;
}

bool op_72(Survivor survivor[static 1], uint16_t shared_memory) // JC,JB,JNAE
{

    bool condition = (sregs.Flags & 0x0001);
    if (condition) general_jmp_near_2B_opcode(survivor);
    else sregs.IP += 2;
    return true;
}

bool op_73(Survivor survivor[static 1], uint16_t shared_memory) // JNC,JNC,JAE
{

    bool condition = !(sregs.Flags & 0x0001);
    if (condition) general_jmp_near_2B_opcode(survivor);
    else sregs.IP += 2;
    return true;
}

bool op_74(Survivor survivor[static 1], uint16_t shared_memory) // JE,JZ
{
    bool condition = (sregs.Flags & 0x0040);
    if (condition) general_jmp_near_2B_opcode(survivor);
    else sregs.IP += 2;
    return true;
}

bool op_75(Survivor survivor[static 1], uint16_t shared_memory) // JNE,JNZ
{

    bool condition = !(sregs.Flags & 0x0040);
    if (condition) general_jmp_near_2B_opcode(survivor);
    else sregs.IP += 2;
    return true;
}

bool op_76(Survivor survivor[static 1], uint16_t shared_memory) // JBE,JNA
{

    bool condition = (sregs.Flags & 0x0001) || (sregs.Flags & 0x0040);
    if (condition) general_jmp_near_2B_opcode(survivor);
    else sregs.IP += 2;
    return true;
}

bool op_77(Survivor survivor[static 1], uint16_t shared_memory) // JNBE,JA
{

    bool condition = !((sregs.Flags & 0x0001) || (sregs.Flags & 0x0040));
    if (condition) general_jmp_near_2B_opcode(survivor);
    else sregs.IP += 2;
    return true;
}

bool op_78(Survivor survivor[static 1], uint16_t shared_memory) // JS
{

    bool condition = (sregs.Flags & 0x0080);
    if (condition) general_jmp_near_2B_opcode(survivor);
    else sregs.IP += 2;
    return true;
}

bool op_79(Survivor survivor[static 1], uint16_t shared_memory) // JNS
{

    bool condition = !(sregs.Flags & 0x0080);
    if (condition) general_jmp_near_2B_opcode(survivor);
    else sregs.IP += 2;
    return true;
}

bool op_7A(Survivor survivor[static 1], uint16_t shared_memory) // JP,JPE
{

    bool condition = (sregs.Flags & 0x0004);
    if (condition) general_jmp_near_2B_opcode(survivor);
    else sregs.IP += 2;
    return true;
}

bool op_7B(Survivor survivor[static 1], uint16_t shared_memory) // JNP,JPO
{

    bool condition = !(sregs.Flags & 0x0004);
    if (condition) general_jmp_near_2B_opcode(survivor);
    else sregs.IP += 2;
    return true;
}

bool op_7C(Survivor survivor[static 1], uint16_t shared_memory) // JL,JNGE
{

    bool condition = ((sregs.Flags & 0x0080) && 1) != ((sregs.Flags & 0x0800) && 1);
    if (condition) general_jmp_near_2B_opcode(survivor);
    else sregs.IP += 2;
    return true;
}

bool op_7D(Survivor survivor[static 1], uint16_t shared_memory) // JNL,JGE
{

    bool condition = ((sregs.Flags & 0x0080) && 1) == ((sregs.Flags & 0x0800) && 1);
    if (condition) general_jmp_near_2B_opcode(survivor);
    else sregs.IP += 2;
    return true;
}

bool op_7E(Survivor survivor[static 1], uint16_t shared_memory) // JLE,JNG
{

    bool condition = (sregs.Flags & 0x0040) || (((sregs.Flags & 0x0080) && 1) != ((sregs.Flags & 0x0800) && 1));
    if (condition) general_jmp_near_2B_opcode(survivor);
    else sregs.IP += 2;
    return true;
}

bool op_7F(Survivor survivor[static 1], uint16_t shared_memory) // JNLE,JG
{

    bool condition = !(sregs.Flags & 0x0040) && ((sregs.Flags & 0x0080) && 1) == ((sregs.Flags & 0x0800) && 1);
    if (condition) general_jmp_near_2B_opcode(survivor);
    else sregs.IP += 2;
    return true;
}

bool op_80(Survivor survivor[static 1], uint16_t shared_memory) // OP byte ptr [X], imm8
{
    uint8_t op_byte = memory[0].values[(sregs.IP + 0x10*sregs.CS + 1) & 0xFFFF];
    operation_ptr op;

    switch ((op_byte >> 3) & 0b111) {
        case 0:
        {
            op = general_add;
            break;
        }
        case 1:
        {
            op = general_or;
            break;
        }
        case 2:
        {
            op = general_adc;
            break;
        }
        case 3:
        {
            op = general_sbb;
            break;
        }
        case 4:
        {
            op = general_and;
            break;
        }
        case 5:
        {
            op = general_sub;
            break;
        }
        case 6:
        {
            op = general_xor;
            break;
        }
        case 7:
        {
            op = general_cmp;
            break;
        }
    }

    return general_op_6(survivor, shared_memory, op);
}

bool op_81(Survivor survivor[static 1], uint16_t shared_memory) // OP byte ptr [X], imm16
{
    uint8_t op_byte = memory[0].values[(sregs.IP + 0x10*sregs.CS + 1) & 0xFFFF];
    operation_ptr op;

    switch ((op_byte >> 3) & 0b111) {
        case 0:
        {
            op = general_add;
            break;
        }
        case 1:
        {
            op = general_or;
            break;
        }
        case 2:
        {
            op = general_adc;
            break;
        }
        case 3:
        {
            op = general_sbb;
            break;
        }
        case 4:
        {
            op = general_and;
            break;
        }
        case 5:
        {
            op = general_sub;
            break;
        }
        case 6:
        {
            op = general_xor;
            break;
        }
        case 7:
        {
            op = general_cmp;
            break;
        }
    }

    return general_op_7(survivor, shared_memory, op);
}

opcode_ptr op_82 = op_80;

bool op_83(Survivor survivor[static 1], uint16_t shared_memory) // OP byte ptr [X], imm16
{
    uint8_t op_byte = memory[0].values[(sregs.IP + 0x10*sregs.CS + 1) & 0xFFFF];
    operation_ptr op;

    switch ((op_byte >> 3) & 0b111) {
        case 0:
        {
            op = general_add;
            break;
        }
        case 1:
        {
            op = general_or;
            break;
        }
        case 2:
        {
            op = general_adc;
            break;
        }
        case 3:
        {
            op = general_sbb;
            break;
        }
        case 4:
        {
            op = general_and;
            break;
        }
        case 5:
        {
            op = general_sub;
            break;
        }
        case 6:
        {
            op = general_xor;
            break;
        }
        case 7:
        {
            op = general_cmp;
            break;
        }
    }

    return general_op_8(survivor, shared_memory, op);
}

bool op_84(Survivor survivor[static 1], uint16_t shared_memory) // TEST reg8, [X]
{

    operation_ptr operation = general_test;
    op_generalizer generalizer = general_op_2;

    return generalizer(survivor, shared_memory, operation);
}

bool op_85(Survivor survivor[static 1], uint16_t shared_memory) // TEST reg16, [X]
{

    operation_ptr operation = general_test;
    op_generalizer generalizer = general_op_3;

    return generalizer(survivor, shared_memory, operation);
}

bool op_86(Survivor survivor[static 1], uint16_t shared_memory) // XCHG reg8, [X]
{

    operation_ptr operation = general_xchg;
    op_generalizer generalizer = general_op_2;

    return generalizer(survivor, shared_memory, operation);
}

bool op_87(Survivor survivor[static 1], uint16_t shared_memory) // XCHG reg16, [X]
{

    operation_ptr operation = general_xchg;
    op_generalizer generalizer = general_op_3;

    return generalizer(survivor, shared_memory, operation);
}

bool general_mov_op(Survivor survivor[static 1], uint16_t shared_memory) // MOV *, * (general_op0/1/2/3)
{

    uint8_t opcode = memory[0].values[sregs.IP + 0x10*sregs.CS];
    operation_ptr operation = general_mov;
    op_generalizer generalizer = general_ops[opcode & 0b111];

    return generalizer(survivor, shared_memory, operation);
}

bool op_8C(Survivor survivor[static 1], uint16_t shared_memory) // MOV [X], seg
{

    operation_ptr operation = general_mov;
    op_generalizer generalizer = general_op_9;

    return generalizer(survivor, shared_memory, operation);
}

bool op_8D(Survivor survivor[static 1], uint16_t shared_memory) // LEA reg16, [X]
{

    uint8_t address_byte = memory[0].values[(sregs.IP + 0x10*sregs.CS + 1) & 0xFFFF];
    uint16_t pos = sregs.IP + 0x10*sregs.CS + 2;

    uint8_t* significant_address,* insignificant_address;

    insignificant_address = (uint8_t*)reg16_decoder(survivor, (address_byte & 0b00111000) >> 3);
    significant_address = insignificant_address + 1;

    uint8_t ip_progress = 0;

    uint16_t segment_register_virtual_addr;
    uint16_t destination_virtual_addr = 0;
    uint8_t* destination = 0;

    if (!get_virtual_address(&ip_progress, &destination, &destination_virtual_addr, survivor, &address_byte, pos, &segment_register_virtual_addr)) {return false;}

    if (destination != 0) {
        return false;
    }

    uint8_t* insignificant_destination = (uint8_t*) &destination_virtual_addr;
    uint8_t* significant_destination = insignificant_destination + 1;

    general_mov(survivor, 1, significant_address, insignificant_address, significant_destination, insignificant_destination);

    sregs.IP += ip_progress;
    return true;
}

bool op_8E(Survivor survivor[static 1], uint16_t shared_memory) // MOV [X], seg
{

    operation_ptr operation = general_mov;
    op_generalizer generalizer = general_op_10;

    return generalizer(survivor, shared_memory, operation);
}

bool op_8F(Survivor survivor[static 1], uint16_t shared_memory) // POP [X]
{

    uint8_t address_byte = memory[0].values[(sregs.IP + 0x10*sregs.CS + 1) & 0xFFFF];
    uint16_t pos = sregs.IP + 0x10*sregs.CS + 2;

    uint8_t ip_progress = 0;

    uint16_t segment_register_virtual_addr;
    uint16_t destination_virtual_addr = 0;
    uint8_t* destination = 0;

    if (!get_virtual_address(&ip_progress, &destination, &destination_virtual_addr, survivor, &address_byte, pos, &segment_register_virtual_addr)) {return false;}

    if (destination != 0) {
        return false;
    }

    uint16_t segment = ((uint32_t) (destination_virtual_addr+0x10*segment_register_virtual_addr) & 0xF0000) >> 16;
    if (segment != 0 && segment != survivor->stack_id && segment != shared_memory) {return false;}

    uint32_t segmented_destination_virtual_addr = destination_virtual_addr + 0x10*segment_register_virtual_addr;
    uint16_t destination_in_memory = memory[0].values[segmented_destination_virtual_addr];

    sregs.IP += ip_progress - 1;

    return general_pop(survivor, shared_memory, &destination_in_memory);
}

bool general_xchg_op(Survivor survivor[static 1], uint16_t shared_memory) // XCHG reg16, AX
{

    uint8_t* insignificant_address = (uint8_t*) &sregs.AX;
    uint8_t* significant_address = insignificant_address + 1;

    uint8_t* insignificant_destination = (uint8_t*) reg16_decoder(survivor, memory[0].values[sregs.IP + 0x10*sregs.CS] & 0b111);
    uint8_t* significant_destination = insignificant_destination + 1;

    general_xchg(survivor, 1, significant_address, insignificant_address,significant_destination, insignificant_destination);

    sregs.IP += 1;

    return true;
}

bool op_98(Survivor survivor[static 1], uint16_t shared_memory) // CBW
{

    if ((int8_t)sregs.AX < 0) {
        sregs.AX |= 0xFF00;
    }
    else {
        sregs.AX &= 0x00FF;
    }

    sregs.IP += 1;

    return true;
}

bool op_99(Survivor survivor[static 1], uint16_t shared_memory) // CBW
{

    if ((int16_t)sregs.AX < 0) {
        sregs.DS = 0xFFFF;
    }
    else {
        sregs.DS &= 0;
    }

    sregs.IP += 1;

    return true;
}

bool op_9A(Survivor survivor[static 1], uint16_t shared_memory) // CALL FAR imm16:imm16
{

    uint16_t new_CS = memory[0].values[(sregs.IP + 0x10*sregs.CS + 1) & 0xFFFF]  + 0x100*memory[0].values[(sregs.IP + 0x10*sregs.CS + 2) & 0xFFFF];
    uint16_t new_IP = memory[0].values[(sregs.IP + 0x10*sregs.CS + 3) & 0xFFFF]  + 0x100*memory[0].values[(sregs.IP + 0x10*sregs.CS + 4) & 0xFFFF];

    sregs.IP += 5;

    general_push(survivor, shared_memory, &sregs.CS);
    general_push(survivor, shared_memory, &sregs.IP);

    sregs.CS = new_CS;
    sregs.IP = new_IP;

    return true;
}

bool op_9B(Survivor survivor[static 1], uint16_t shared_memory) // original: WAIT, modified: virtual opcode NRG
{

    if ((memory[0].values[(sregs.IP + 0x10*sregs.CS + 1) & 0xFFFF]) != 0x9b) {return false;}
    if (sregs.Energy != 0xFFFF) sregs.Energy++;
    sregs.IP += 2;

    return true;
}

bool op_9C(Survivor survivor[static 1], uint16_t shared_memory) // Push CS
{

    return general_push(survivor, shared_memory, &sregs.Flags);
}

bool op_9D(Survivor survivor[static 1], uint16_t shared_memory) // Push SS
{

    return general_push(survivor, shared_memory, &sregs.Flags);
}

bool op_9E(Survivor survivor[static 1], uint16_t shared_memory) // SAHF
{

    *(uint8_t*)&sregs.Flags = *((uint8_t*)&sregs.AX + 1);

    return true;
}

bool op_9F(Survivor survivor[static 1], uint16_t shared_memory) // LAHF
{

     *((uint8_t*)&sregs.AX + 1) = *(uint8_t*)&sregs.Flags;

    return true;
}

bool op_A0(Survivor survivor[static 1], uint16_t shared_memory) // MOV AL, [imm16]
{

    uint32_t address = 0x10*sregs.DS + memory[0].values[(sregs.IP + 0x10*sregs.CS + 1) & 0xFFFF] + 0x10*memory[0].values[(sregs.IP + 0x10*sregs.CS + 2) & 0xFFFF];

    uint16_t segment = (address & 0xFF0000) >> 16;
    if (segment != 0 && segment != survivor->stack_id && segment != shared_memory) {return false;}

    *(uint8_t*) &sregs.AX = memory[0].values[address];

    sregs.IP += 3;

    return true;
}

bool op_A1(Survivor survivor[static 1], uint16_t shared_memory) // MOV AX, [imm16]
{

    uint32_t address = 0x10*sregs.DS + memory[0].values[(sregs.IP + 0x10*sregs.CS + 1) & 0xFFFF] + 0x10*memory[0].values[(sregs.IP + 0x10*sregs.CS + 2) & 0xFFFF];

    uint16_t segment = (address & 0xFF0000) >> 16;
    if (segment != 0 && segment != survivor->stack_id && segment != shared_memory) {return false;}

    sregs.AX = memory[0].values[address];

    sregs.IP += 3;

    return true;
}

bool op_A2(Survivor survivor[static 1], uint16_t shared_memory) // MOV [imm16], AL
{

    uint32_t destination =0x10*sregs.DS + memory[0].values[(sregs.IP + 0x10*sregs.CS + 1) & 0xFFFF] + 0x10*memory[0].values[(sregs.IP + 0x10*sregs.CS + 2) & 0xFFFF];

    uint16_t segment = (destination & 0xFF0000) >> 16;
    if (segment != 0 && segment != survivor->stack_id && segment != shared_memory) {return false;}

     memory[0].values[destination] = *(uint8_t*) &sregs.AX;

    sregs.IP += 3;

    return true;
}

bool op_A3(Survivor survivor[static 1], uint16_t shared_memory) // MOV [imm16], AX
{

    uint32_t destination = 0x10*sregs.DS + memory[0].values[(sregs.IP + 0x10*sregs.CS + 1) & 0xFFFF] + 0x10*memory[0].values[(sregs.IP + 0x10*sregs.CS + 2) & 0xFFFF];

    uint16_t segment = (destination & 0xFF0000) >> 16;
    if (segment != 0 && segment != survivor->stack_id && segment != shared_memory) {return false;}

    *(uint16_t*) &memory[0].values[destination] = sregs.AX;

    sregs.IP += 3;

    return true;
}

// Might be a diference in implementation, here we assume the opcode keeps the text in the same page.
bool op_A4(Survivor survivor[static 1], uint16_t shared_memory) // MOVSB
{

    uint32_t address = sregs.SI + 0x10*sregs.DS;

    uint16_t addr_segment = (address & 0xFF0000) >> 16;
    if (addr_segment != 0 && addr_segment != survivor->stack_id && addr_segment != shared_memory) {return false;}


    uint32_t destination = sregs.DI + 0x10*sregs.ES;

    uint16_t dest_segment = (destination & 0xFF0000) >> 16;
    if (dest_segment != 0 && dest_segment != survivor->stack_id && dest_segment != shared_memory) {return false;}

    memory[0].values[destination] = memory[0].values[address];

    int8_t diff = (sregs.Flags & 0x0400) ? -1 : 1;

    sregs.DI += diff;
    sregs.SI += diff;

    sregs.IP += 1;

    return true;
}

bool op_A5(Survivor survivor[static 1], uint16_t shared_memory) // MOVSW
{

    uint32_t address = sregs.SI + 0x10*sregs.DS;

    uint16_t addr_segment = (address & 0xFF0000) >> 16;
    if (addr_segment != 0 && addr_segment != survivor->stack_id && addr_segment != shared_memory) {return false;}


    uint32_t destination = sregs.DI + 0x10*sregs.ES;

    uint16_t dest_segment = (destination & 0xFF0000) >> 16;
    if (dest_segment != 0 && dest_segment != survivor->stack_id && dest_segment != shared_memory) {return false;}

    memory[0].values[destination] = memory[0].values[address];
    memory[0].values[((destination + 1)&0xFFFF) + 0x10000*dest_segment] = memory[0].values[((address + 1)&0xFFFF) + 0x10000*addr_segment];

    int8_t diff = (sregs.Flags & 0x0400) ? -2 : 2;

    sregs.DI += diff;
    sregs.SI += diff;

    sregs.IP += 1;

    return true;
}

bool op_A6(Survivor survivor[static 1], uint16_t shared_memory) // CMPSB
{

    uint32_t address = sregs.SI + 0x10*sregs.DS;

    uint16_t addr_segment = (address & 0xFF0000) >> 16;
    if (addr_segment != 0 && addr_segment != survivor->stack_id && addr_segment != shared_memory) {return false;}


    uint32_t destination = sregs.DI + 0x10*sregs.ES;

    uint16_t dest_segment = (destination & 0xFF0000) >> 16;
    if (dest_segment != 0 && dest_segment != survivor->stack_id && dest_segment != shared_memory) {return false;}

    general_cmp(survivor, 0, &memory[0].values[address], 0, &memory[0].values[destination], 0);

    int8_t diff = (sregs.Flags & 0x0400) ? -1 : 1;

    sregs.DI += diff;
    sregs.SI += diff;

    sregs.IP += 1;

    return true;
}

bool op_A7(Survivor survivor[static 1], uint16_t shared_memory) // CMPSW
{

    uint32_t address = sregs.SI + 0x10*sregs.DS;

    uint16_t addr_segment = (address & 0xFF0000) >> 16;
    if (addr_segment != 0 && addr_segment != survivor->stack_id && addr_segment != shared_memory) {return false;}


    uint32_t destination = sregs.DI + 0x10*sregs.ES;

    uint16_t dest_segment = (destination & 0xFF0000) >> 16;
    if (dest_segment != 0 && dest_segment != survivor->stack_id && dest_segment != shared_memory) {return false;}

    general_cmp(survivor, 1, &memory[0].values[((address + 1)&0xFFFF) + 0x10000*addr_segment],
                &memory[0].values[address], &memory[0].values[((destination + 1)&0xFFFF) + 0x10000*dest_segment], &memory[0].values[destination]);

    int8_t diff = (sregs.Flags & 0x0400) ? -2 : 2;

    sregs.DI += diff;
    sregs.SI += diff;

    sregs.IP += 1;

    return true;
}

bool op_A8(Survivor survivor[static 1], uint16_t shared_memory) // TEST AL, imm8
{

    operation_ptr operation = general_test;
    op_generalizer generalizer = general_op_4;

    return generalizer(survivor, shared_memory, operation);
}

bool op_A9(Survivor survivor[static 1], uint16_t shared_memory) // TEST AX, imm16
{

    operation_ptr operation = general_test;
    op_generalizer generalizer = general_op_4;

    return generalizer(survivor, shared_memory, operation);
}

bool op_AA(Survivor survivor[static 1], uint16_t shared_memory) // STOSB
{

    uint8_t address_value = (uint8_t)sregs.AX;

    uint32_t destination = sregs.DI + 0x10*sregs.ES;

    uint16_t dest_segment = (destination & 0xFF0000) >> 16;
    if (dest_segment != 0 && dest_segment != survivor->stack_id && dest_segment != shared_memory) {return false;}

    memory[0].values[destination] = address_value;

    int8_t diff = (sregs.Flags & 0x0400) ? -1 : 1;
    sregs.DI += diff;

    sregs.IP += 1;

    return true;
}

bool op_AB(Survivor survivor[static 1], uint16_t shared_memory) // STOSW
{

    uint8_t* address = (uint8_t*) &sregs.AX;


    uint32_t destination = sregs.DI + 0x10*sregs.ES;

    uint16_t dest_segment = (destination & 0xFF0000) >> 16;
    if (dest_segment != 0 && dest_segment != survivor->stack_id && dest_segment != shared_memory) {return false;}

    memory[0].values[destination] = *address;
    memory[0].values[((destination + 1)&0xFFFF) + 0x10000*dest_segment] = *(address + 1);

    int8_t diff = (sregs.Flags & 0x0400) ? -2 : 2;
    sregs.DI += diff;

    sregs.IP += 1;

    return true;
}

bool op_AC(Survivor survivor[static 1], uint16_t shared_memory) // LODSB
{

    uint32_t address = sregs.SI + 0x10*sregs.DS;

    uint16_t addr_segment = (address & 0xFF0000) >> 16;
    if (addr_segment != 0 && addr_segment != survivor->stack_id && addr_segment != shared_memory) {return false;}


    uint8_t* destination = (uint8_t*) &sregs.AX;

    *destination = memory[0].values[address];

    int8_t diff = (sregs.Flags & 0x0400) ? -1 : 1;
    sregs.SI += diff;

    sregs.IP += 1;

    return true;
}

bool op_AD(Survivor survivor[static 1], uint16_t shared_memory) // LODSW
{

    uint32_t address = sregs.SI + 0x10*sregs.DS;

    uint16_t addr_segment = (address & 0xFF0000) >> 16;
    if (addr_segment != 0 && addr_segment != survivor->stack_id && addr_segment != shared_memory) {return false;}


    uint8_t* destination = (uint8_t*) &sregs.AX;

    *destination = memory[0].values[address];
    *(destination + 1) = memory[0].values[((address + 1)&0xFFFF) + 0x10000*addr_segment];

    int8_t diff = (sregs.Flags & 0x0400) ? -2 : 2;
    sregs.SI += diff;

    sregs.IP += 1;

    return true;
}

bool op_AE(Survivor survivor[static 1], uint16_t shared_memory) // SCASB
{

    uint8_t* address = (uint8_t*) &sregs.AX;

    uint32_t destination = sregs.DI + 0x10*sregs.ES;

    uint16_t dest_segment = (destination & 0xFF0000) >> 16;
    if (dest_segment != 0 && dest_segment != survivor->stack_id && dest_segment != shared_memory) {return false;}

    general_cmp(survivor, 0, address, 0, &memory[0].values[destination], 0);

    int8_t diff = (sregs.Flags & 0x0400) ? -1 : 1;
    sregs.DI += diff;

    sregs.IP += 1;

    return true;
}

bool op_AF(Survivor survivor[static 1], uint16_t shared_memory) // SCASW
{

    uint8_t* address = (uint8_t*) &sregs.AX;

    uint32_t destination = sregs.DI + 0x10*sregs.ES;

    uint16_t dest_segment = (destination & 0xFF0000) >> 16;
    if (dest_segment != 0 && dest_segment != survivor->stack_id && dest_segment != shared_memory) {return false;}

    general_cmp(survivor, 1, address+1,address,
                &memory[0].values[((destination + 1)&0xFFFF) + 0x10000*dest_segment], &memory[0].values[destination]);

    int8_t diff = (sregs.Flags & 0x0400) ? -2 : 2;
    sregs.DI += diff;

    sregs.IP += 1;

    return true;
}

bool op_B0to7(Survivor survivor[static 1], uint16_t shared_memory) // MOV reg8, imm8
{

    operation_ptr operation = general_mov;
    op_generalizer generalizer = general_op_11;

    return generalizer(survivor, shared_memory, operation);
}

bool op_B8toF(Survivor survivor[static 1], uint16_t shared_memory) // MOV reg16, imm16
{

    operation_ptr operation = general_mov;
    op_generalizer generalizer = general_op_12;

    return generalizer(survivor, shared_memory, operation);
}

bool op_C2(Survivor survivor[static 1], uint16_t shared_memory) // RETN [imm16]
{

    uint16_t size_to_pop = memory[0].values[sregs.IP + 0x10*sregs.CS] + 0x10*memory[0].values[(sregs.IP + 0x10*sregs.CS + 1) & 0xFFFF];

    if (!general_pop(survivor, shared_memory, &sregs.IP)) {return false;}

    sregs.SP = sregs.SP + size_to_pop;

    return true;

    // TODO: no incrementing IP?
}

bool op_C3(Survivor survivor[static 1], uint16_t shared_memory) // RETN
{

    return general_pop(survivor, shared_memory, &sregs.IP);

    // TODO: no incrementing IP?
}

bool op_C4(Survivor survivor[static 1], uint16_t shared_memory) // LES reg16, [X]
{

    uint8_t address_byte = memory[0].values[(sregs.IP + 0x10*sregs.CS + 1) & 0xFFFF];
    uint16_t pos = sregs.IP + 0x10*sregs.CS + 2;

    uint8_t ip_progress = 0;

    uint16_t segment_register_virtual_addr;
    uint16_t address_virtual_addr = 0;
    uint8_t* address = 0;

    if (!get_virtual_address(&ip_progress, &address, &address_virtual_addr, survivor, &address_byte, pos, &segment_register_virtual_addr)) {return false;}

    if (address != 0) {
        return false;
    }

    uint32_t real_virtual_addr = (uint32_t) address_virtual_addr + 0x10*segment_register_virtual_addr;
    uint16_t addr_segment = (real_virtual_addr  & 0xFF0000) >> 16;
    if (addr_segment != 0 && addr_segment != survivor->stack_id && addr_segment != shared_memory) {return false;}

    uint8_t* reg = (uint8_t*) reg16_decoder(survivor, (address_byte & 0b111000) >> 3);
    uint8_t* seg_reg = (uint8_t*) &sregs.ES;

    *reg = memory[0].values[(address_virtual_addr + 0x10*segment_register_virtual_addr) & 0xFFFF];
    *(reg + 1) = memory[0].values[(address_virtual_addr + 0x10*segment_register_virtual_addr + 1) & 0xFFFF];
    *(seg_reg) = memory[0].values[(address_virtual_addr + 0x10*segment_register_virtual_addr + 2) & 0xFFFF];
    *(seg_reg + 1) = memory[0].values[(address_virtual_addr + 0x10*segment_register_virtual_addr + 3) & 0xFFFF];

    sregs.IP += ip_progress;

    return true;
}

bool op_C5(Survivor survivor[static 1], uint16_t shared_memory) // LDS reg16, [X]
{

    uint8_t address_byte = memory[0].values[(sregs.IP + 0x10*sregs.CS + 1) & 0xFFFF];
    uint16_t pos = sregs.IP + 0x10*sregs.CS + 2;

    uint8_t ip_progress = 0;

    uint16_t segment_register_virtual_addr;
    uint16_t address_virtual_addr = 0;
    uint8_t* address = 0;

    if (!get_virtual_address(&ip_progress, &address, &address_virtual_addr, survivor, &address_byte, pos, &segment_register_virtual_addr)) {return false;}

    if (address != 0) {
        return false;
    }

    uint32_t real_virtual_addr = (uint32_t) address_virtual_addr + 0x10*segment_register_virtual_addr;
    uint16_t addr_segment = (real_virtual_addr  & 0xFF0000) >> 16;
    if (addr_segment != 0 && addr_segment != survivor->stack_id && addr_segment != shared_memory) {return false;}

    uint8_t* reg = (uint8_t*) reg16_decoder(survivor, (address_byte & 0b111000) >> 3);
    uint8_t* seg_reg = (uint8_t*) &sregs.DS;

    *reg = memory[0].values[(address_virtual_addr + 0x10*segment_register_virtual_addr) & 0xFFFF];
    *(reg + 1) = memory[0].values[(address_virtual_addr + 0x10*segment_register_virtual_addr + 1) & 0xFFFF];
    *(seg_reg) = memory[0].values[(address_virtual_addr + 0x10*segment_register_virtual_addr + 2) & 0xFFFF];
    *(seg_reg + 1) = memory[0].values[(address_virtual_addr + 0x10*segment_register_virtual_addr + 3) & 0xFFFF];

    sregs.IP += ip_progress;

    return true;
}

bool op_C6(Survivor survivor[static 1], uint16_t shared_memory) // MOV [X], imm8
{

    operation_ptr operation = general_mov;
    op_generalizer generalizer = general_op_6;

    return generalizer(survivor, shared_memory, operation);
}

bool op_C7(Survivor survivor[static 1], uint16_t shared_memory) // MOV [X], imm16
{

    operation_ptr operation = general_mov;
    op_generalizer generalizer = general_op_7;

    return generalizer(survivor, shared_memory, operation);
}

bool op_CA(Survivor survivor[static 1], uint16_t shared_memory) //RETF [imm16]
{

    uint16_t size_to_pop = memory[0].values[sregs.IP + 0x10*sregs.CS] + 0x10*memory[0].values[(sregs.IP + 0x10*sregs.CS + 1) & 0xFFFF];

    if (!general_pop(survivor, shared_memory, &sregs.IP)) {return false;}
    if (!general_pop(survivor, shared_memory, &sregs.CS)) {return false;}

    sregs.SP = sregs.SP + size_to_pop;

    return true;

    // TODO: no incrementing IP?
}

bool op_CB(Survivor survivor[static 1], uint16_t shared_memory) // RETN
{

    if (!general_pop(survivor, shared_memory, &sregs.IP)) {return false;}
    return general_pop(survivor, shared_memory, &sregs.CS);

    // TODO: no incrementing IP?
}


bool op_CD(Survivor survivor[static 1], uint16_t shared_memory) // INT [imm8]
{

    uint8_t interrupt_number = memory[0].values[sregs.IP + 0x10*sregs.CS];
    if (interrupt_number == 0x86) {
        if (sregs.INT86h_used >= 2) {return false;}
        for (int i=0; i<=127; i++) {op_AB(survivor, shared_memory);}
        sregs.INT86h_used++;
        return true;

    }
    else if (interrupt_number == 0x87) {
        if (sregs.INT86h_used >= 1) {return false;}
        for (int i=0; i<=0xFFFF; i++)
        {
            // might be a difference, here we are "nicer" with checking forbidden memory.
            int diff = (sregs.Flags & 0x0400) ? -i : i;
            int16_t address = 0x10*sregs.ES + ((sregs.DS + diff) & 0xFFFF);
            int16_t segment = ((0x10*sregs.ES + ((sregs.DS + diff) & 0xFFFF)) >> 16);
            if ((memory[0].values[(uint32_t) (segment<<16) + address] == (uint8_t) sregs.AX)
                &&
                (memory[0].values[(uint32_t) (segment<<16) + ((address + 1) & 0xFFFF)] == *(((uint8_t*) &sregs.AX) + 1)))
            {
                if ((memory[0].values[(uint32_t) (segment<<16) + ((address + 2) & 0xFFFF)] == (uint8_t) sregs.DX)
                    &&
                    (memory[0].values[(uint32_t) (segment<<16) + ((address + 3) & 0xFFFF)] == *(((uint8_t*) &sregs.DX) + 1)))
                {
                    if (segment != 0 && segment != survivor->stack_id && segment != shared_memory) {return false;}

                    memory[0].values[(uint32_t) (segment<<16) + ((address + 0) & 0xFFFF)] = (uint8_t) sregs.BX;
                    memory[0].values[(uint32_t) (segment<<16) + ((address + 1) & 0xFFFF)] = *(((uint8_t*) &sregs.BX) + 1);
                    memory[0].values[(uint32_t) (segment<<16) + ((address + 2) & 0xFFFF)] = (uint8_t) sregs.CX;
                    memory[0].values[(uint32_t) (segment<<16) + ((address + 3) & 0xFFFF)] = *(((uint8_t*) &sregs.CX) + 1);
                }
            }
        }

        sregs.INT87h_used++;
        return true;

    } else {return false;}
}

bool op_CF(Survivor survivor[static 1], uint16_t shared_memory) // IRET
{

    if (!general_pop(survivor, shared_memory, &sregs.IP)) {return false;}
    if (!general_pop(survivor, shared_memory, &sregs.CS)) {return false;}
    return general_pop(survivor, shared_memory, &sregs.Flags);

    // TODO: no incrementing IP?
}

bool op_D0(Survivor survivor[static 1], uint16_t shared_memory) // <?> byte ptr [X], 1
{

    static operation_ptr ops[] = {general_rol,
                                  general_ror,
                                  general_rcl,
                                  general_rcr,
                                  general_shl,
                                  general_shr,
                                  0,
                                  general_sar};
    uint8_t op_byte = (memory[0].values[sregs.IP + 0x10*sregs.CS] >> 3) & 0b111;

    operation_ptr operation;
    if ((operation = ops[op_byte]) == 0) {return false;}

    op_generalizer generalizer = general_op_13;

    return generalizer(survivor, shared_memory, operation);
}

bool op_D1(Survivor survivor[static 1], uint16_t shared_memory) // <?> word ptr [X], 1
{

    static operation_ptr ops[] = {general_rol,
                                  general_ror,
                                  general_rcl,
                                  general_rcr,
                                  general_shl,
                                  general_shr,
                                  0,
                                  general_sar};
    uint8_t op_byte = (memory[0].values[sregs.IP +0x10*sregs.CS] >> 3) & 0b111;

    operation_ptr operation;
    if ((operation = ops[op_byte]) == 0) {return false;}

    op_generalizer generalizer = general_op_14;

    return generalizer(survivor, shared_memory, operation);
}

bool op_D2(Survivor survivor[static 1], uint16_t shared_memory) // <?> byte ptr [X], CL
{

    static operation_ptr ops[] = {general_rol,
                                  general_ror,
                                  general_rcl,
                                  general_rcr,
                                  general_shl,
                                  general_shr,
                                  0,
                                  general_sar};
    uint8_t op_byte = (memory[0].values[sregs.IP + 0x10*sregs.CS] >> 3) & 0b111;

    operation_ptr operation;
    if ((operation = ops[op_byte]) == 0) {return false;}

    op_generalizer generalizer = general_op_15;

    return generalizer(survivor, shared_memory, operation);
}

bool op_D3(Survivor survivor[static 1], uint16_t shared_memory) // <?> word ptr [X], CL
{

    static operation_ptr ops[] = {general_rol,
                                  general_ror,
                                  general_rcl,
                                  general_rcr,
                                  general_shl,
                                  general_shr,
                                  0,
                                  general_sar};
    uint8_t op_byte = (memory[0].values[sregs.IP + 0x10*sregs.CS] >> 3) & 0b111;

    operation_ptr operation;
    if ((operation = ops[op_byte]) == 0) {return false;}

    op_generalizer generalizer = general_op_16;

    return generalizer(survivor, shared_memory, operation);
}

bool op_D7(Survivor survivor[static 1], uint16_t shared_memory) // XLAT, XLATB
{

    uint16_t address_virtual_addr = memory[0].values[(sregs.IP + 0x10*sregs.CS + 2) & 0xFFFF] + 0x100*memory[0].values[(sregs.IP + 0x10*sregs.CS + 3) & 0xFFFF];
    uint16_t segment_register_virtual_addr = sregs.DS;
    uint32_t real_address = (uint32_t) address_virtual_addr + 0x10 * segment_register_virtual_addr;

    uint16_t segment = (real_address >> 16);
    if (segment != 0 && segment != survivor->stack_id && segment != shared_memory) {return false;}

    *(uint8_t*) &sregs.AX = memory[0].values[real_address];

    sregs.IP += 1;
    return true;
}

bool op_E0(Survivor survivor[static 1], uint16_t shared_memory) // LOOPNZ, LOOPNE
{

    bool condition = (--sregs.CX) && (sregs.Flags & 0x0040);
    if (condition) general_jmp_near_2B_opcode(survivor);

    else sregs.IP += 2;
    return true;
}

bool op_E1(Survivor survivor[static 1], uint16_t shared_memory) // LOOPZ, LOOPE
{

    bool condition = (--sregs.CX) && !(sregs.Flags & 0x0040);
    if (condition) general_jmp_near_2B_opcode(survivor);

    else sregs.IP += 2;
    return true;
}

bool op_E2(Survivor survivor[static 1], uint16_t shared_memory) // LOOP
{

    bool condition = (--sregs.CX);
    if (condition) general_jmp_near_2B_opcode(survivor);

    else sregs.IP += 2;
    return true;
}

bool op_E3(Survivor survivor[static 1], uint16_t shared_memory) // JCXZ
{

    bool condition = !sregs.CX;
    if (condition) general_jmp_near_2B_opcode(survivor);

    else sregs.IP += 2;
    return true;
}

bool op_E8(Survivor survivor[static 1], uint16_t shared_memory) // CALL near imm16
{

    uint16_t new_IP = sregs.IP + memory[0].values[(sregs.IP + 0x10*sregs.CS + 1) & 0xFFFF]  + 0x100*memory[0].values[(sregs.IP + 0x10*sregs.CS + 2) & 0xFFFF];

    sregs.IP += 3;

    general_push(survivor, shared_memory, &sregs.IP);

    sregs.IP = new_IP;

    return true;
}

bool op_E9(Survivor survivor[static 1], uint16_t shared_memory) // JMP near imm16
{

    uint16_t new_IP = memory[0].values[(sregs.IP + 0x10*sregs.CS + 1) & 0xFFFF]  + 0x100*memory[0].values[(sregs.IP + 0x10*sregs.CS + 2) & 0xFFFF];
    sregs.IP = new_IP;

    return true;
}

bool op_EA(Survivor survivor[static 1], uint16_t shared_memory) // JMP FAR imm16:imm16
{

    uint16_t new_CS = memory[0].values[(sregs.IP + 0x10*sregs.CS + 1) & 0xFFFF]  + 0x100*memory[0].values[(sregs.IP + 0x10*sregs.CS + 2) & 0xFFFF];
    uint16_t new_IP = memory[0].values[(sregs.IP + 0x10*sregs.CS + 3) & 0xFFFF]  + 0x100*memory[0].values[(sregs.IP + 0x10*sregs.CS + 4) & 0xFFFF];

    sregs.CS = new_CS;
    sregs.IP = new_IP;

    return true;
}

bool op_EB(Survivor survivor[static 1], uint16_t shared_memory) // JMP short imm8
{

    int8_t IP_offset = memory[0].values[(sregs.IP + 0x10*sregs.CS + 1) & 0xFFFF];
    sregs.IP += IP_offset;

    return true;
}

bool op_F2(Survivor survivor[static 1], uint16_t shared_memory) // REPNZ
{
    uint8_t operation_byte = memory[0].values[(sregs.IP + 0x10*sregs.CS + 1) & 0xFFFF];
    opcode_ptr operation_arr[] = {[0xA6] = op_A6, [0xA7] = op_A7, [0xAE] = op_AE, [0xAF] = op_AF, [0xFF] = 0};
    opcode_ptr operation = operation_arr[operation_byte];
    if (operation == 0) {return false;}

    if (sregs.CX && (sregs.Flags & 0x0040)) {
        if (!operation(survivor, shared_memory)) {return false;}
        sregs.IP--;
        sregs.CX--;
    }
    else {
        sregs.IP += 2;
    }

    return true;
}

bool op_F3(Survivor survivor[static 1], uint16_t shared_memory) // REP, REPZ
{
    uint8_t operation_byte = memory[0].values[(sregs.IP + 0x10*sregs.CS + 1) & 0xFFFF];
    opcode_ptr operation_arr[] = {[0xA4] = op_A4, [0xA5] = op_A5, [0xA6] = op_A6, [0xA7] = op_A7, [0xAA] = op_AA,
                            [0xAB] = op_AB, [0xAC] = op_AC, [0xAD] = op_AD, [0xAE] = op_AE, [0xAF] = op_AF, [0xFF] = 0};
    opcode_ptr operation = operation_arr[operation_byte];
    if (operation == 0) {return false;}
    sregs.IP++;

    if (sregs.CX && (sregs.Flags & 0x0040)) {
        if (!operation(survivor, shared_memory)) {return false;}
        sregs.IP--;
        sregs.CX--;
    }
    else {
        sregs.IP += 2;
    }

    return true;
}

bool op_F5(Survivor survivor[static 1], uint16_t shared_memory) // CMC
{
    sregs.Flags ^= 0x1;
    sregs.IP += 1;
    return true;
}

bool op_F6(Survivor survivor[static 1], uint16_t shared_memory) // OP byte ptr [X]
{
    uint8_t address_byte = memory[0].values[(sregs.IP + 10*sregs.CS + 1) & 0xFFFF];
    uint16_t pos = sregs.IP + 10*sregs.CS + 2;

    uint8_t ip_progress = 0;

    uint16_t destination_virtual_addr = 0;
    uint16_t segment_register_virtual_addr = 0;
    uint8_t* destination = 0;

    if (!get_virtual_address(&ip_progress, &destination, &destination_virtual_addr, survivor, &address_byte, pos, &segment_register_virtual_addr)) {return false;}

    if (destination == 0) {
        uint16_t segment = ((uint32_t) (destination_virtual_addr + 0x10 * segment_register_virtual_addr) >> 16);
        if (segment != 0 && segment != survivor->stack_id && segment != shared_memory) {return false;}

        destination = (uint8_t*)&((uint8_t*) memory)[(uint32_t) destination_virtual_addr + segment_register_virtual_addr * 0x10];
    }

    sregs.IP += ip_progress;

    switch ((address_byte & 0b111000) >> 3) {
        case 0b001: // TEST imm8
        {
            uint8_t address_value = memory[0].values[(sregs.IP + 10*sregs.CS) & 0xFFFF];
            general_test(survivor, 0, &address_value, 0, destination, 0);

            sregs.IP++;
            return true;
        }

        case 0b010: // NOT
        {
            *destination ^= 0xff;

            return true;
        }

        case 0b011: // NEG
        {
            uint8_t pseudo_destination = 0;
            general_sub(survivor, 0, destination, 0, &pseudo_destination, 0);

            *destination = pseudo_destination;
            return true;
        }

        case 0b100: // MUL
        {
            general_mul(survivor, 0, destination, 0);

            return true;
        }

        case 0b101: // (unimplemented) IMUL
        {
            return false;
        }

        case 0b110: // DIV
        {
            if (*destination == 0) {return false;}

            general_div(survivor, 0, destination, 0);

            return true;
        }

        case 0b111: // (unimplemented) IDIV
        {
            return false;
        }
    }

    return false;
}

bool op_F7(Survivor survivor[static 1], uint16_t shared_memory) // OP word ptr [X]
{
    uint8_t address_byte = memory[0].values[(sregs.IP + 10*sregs.CS + 1) & 0xFFFF];
    uint16_t pos = sregs.IP + 10*sregs.CS + 2;
    uint8_t ip_progress = 0;

    uint16_t destination_virtual_addr = 0;
    uint16_t segment_register_virtual_addr = 0;
    uint8_t* insignificant_destination = 0;
    uint8_t* significant_destination;

    if (!get_virtual_address(&ip_progress, &insignificant_destination, &destination_virtual_addr, survivor, &address_byte, pos, &segment_register_virtual_addr)) {return false;}

    if (insignificant_destination == 0) {
        uint16_t segment = ((uint32_t) (destination_virtual_addr + 0x10 * segment_register_virtual_addr) >> 16);
        if (segment != 0 && segment != survivor->stack_id && segment != shared_memory) {return false;}

        insignificant_destination = &((uint8_t*) memory)[(uint32_t) destination_virtual_addr + segment_register_virtual_addr * 0x10];
        significant_destination = &((uint8_t*) memory)[(uint32_t) ((destination_virtual_addr+1)&0xFFFF) + segment_register_virtual_addr * 0x10];
    }
    else {
        significant_destination = insignificant_destination + 1;
    }

    sregs.IP += ip_progress;

    switch ((address_byte & 0b111000) >> 3) {
        case 0b001: // TEST imm8
        {
            uint8_t insignificant_address_value = memory[0].values[(sregs.IP + 10*sregs.CS) & 0xFFFF];
            uint8_t significant_address_value = memory[0].values[(sregs.IP + 10*sregs.CS + 1) & 0xFFFF];
            general_test(survivor, 1, &significant_address_value,&insignificant_address_value,
                         significant_destination, insignificant_destination);

            sregs.IP += 2;
            return true;
        }

        case 0b010: // NOT
        {
            *insignificant_destination ^= 0xff;
            *significant_destination ^= 0xff;

            return true;
        }

        case 0b011: // NEG
        {
            uint8_t insignificant_pseudo_destination = 0;
            uint8_t significant_pseudo_destination = 0;
            general_sub(survivor, 1, significant_destination, insignificant_destination,
                        &significant_pseudo_destination, &insignificant_pseudo_destination);

            *insignificant_destination = insignificant_pseudo_destination;
            *significant_destination = significant_pseudo_destination;
            return true;
        }

        case 0b100: // MUL
        {
            general_mul(survivor, 1, significant_destination, insignificant_destination);

            return true;
        }

        case 0b101: // (unimplemented) IMUL
        {
            return false;
        }

        case 0b110: // DIV
        {
            if ((*insignificant_destination == 0) && (*significant_destination == 0)) {return false;}

            general_div(survivor, 1, significant_destination, insignificant_destination);

            return true;
        }

        case 0b111: // (unimplemented) IDIV
        {
            return false;
        }
    }

    return false;
}

bool op_F8(Survivor survivor[static 1], uint16_t shared_memory) // CLC
{
    sregs.Flags &= (0xFFFF - 0x1);
    sregs.IP++;
    return true;
}

bool op_F9(Survivor survivor[static 1], uint16_t shared_memory) // STC
{
    sregs.Flags |= 0x1;
    sregs.IP++;
    return true;
}

bool op_FA(Survivor survivor[static 1], uint16_t shared_memory) // CLI
{
    sregs.Flags &= (0xFFFF - 0x200);
    sregs.IP++;
    return true;
}

bool op_FB(Survivor survivor[static 1], uint16_t shared_memory) // STI
{
    sregs.Flags |= 0x200;
    sregs.IP++;
    return true;
}

bool op_FC(Survivor survivor[static 1], uint16_t shared_memory) // CLD
{
    sregs.Flags &= (0xFFFF - 0x400);
    sregs.IP++;
    return true;
}

bool op_FD(Survivor survivor[static 1], uint16_t shared_memory) // STD
{
    sregs.Flags |= 0x400;
    sregs.IP++;
    return true;
}

bool op_FE(Survivor survivor[static 1], uint16_t shared_memory) // OP byte ptr [X]
{
    uint8_t address_byte = memory[0].values[(sregs.IP + 10*sregs.CS + 1) & 0xFFFF];
    uint16_t pos = sregs.IP + 10*sregs.CS + 2;
    uint8_t ip_progress = 0;

    uint16_t destination_virtual_addr = 0;
    uint16_t segment_register_virtual_addr = 0;
    uint8_t* destination = 0;

    if (!get_virtual_address(&ip_progress, &destination, &destination_virtual_addr, survivor, &address_byte, pos, &segment_register_virtual_addr)) {return false;}

    if (destination == 0) {
        uint16_t segment = ((uint32_t) (destination_virtual_addr + 0x10 * segment_register_virtual_addr) >> 16);
        if (segment != 0 && segment != survivor->stack_id && segment != shared_memory) {return false;}

        destination = (uint8_t*)&((uint8_t*) memory)[(uint32_t) destination_virtual_addr + segment_register_virtual_addr * 0x10];
    }

    sregs.IP += ip_progress;

    if ((address_byte >> 3) == 0) {
        uint8_t one = 1;
        general_add(survivor, 0, &one, 0,
                    &memory[0].values[(uint32_t) (destination_virtual_addr + 0x10 * segment_register_virtual_addr)], 0);
        return true;
    }
    else if ((address_byte >> 3) == 1) {
        uint8_t one = 1;
        general_sub(survivor, 0, &one, 0,
                    &memory[0].values[(uint32_t) (destination_virtual_addr + 0x10 * segment_register_virtual_addr)], 0);
        return true;
    }
    else {
        return false;
    }
}

bool op_FF(Survivor survivor[static 1], uint16_t shared_memory) // OP word ptr [X]
{
    uint8_t address_byte = memory[0].values[(sregs.IP + 10*sregs.CS + 1) & 0xFFFF];
    uint16_t pos = sregs.IP + 10*sregs.CS + 2;

    uint8_t ip_progress = 0;

    uint16_t destination_virtual_addr = 0;
    uint16_t segment_register_virtual_addr = 0;
    uint8_t* insignificant_destination = 0;
    uint8_t* significant_destination;

    if (!get_virtual_address(&ip_progress, &insignificant_destination, &destination_virtual_addr, survivor, &address_byte, pos, &segment_register_virtual_addr)) {return false;}

    uint16_t real_virtual_addr = (destination_virtual_addr + 0x10 * segment_register_virtual_addr);
    uint16_t segment = ((uint32_t) (destination_virtual_addr + 0x10 * segment_register_virtual_addr) >> 16);
    if (insignificant_destination == 0) {
        if (segment != 0 && segment != survivor->stack_id && segment != shared_memory) {return false;}

        insignificant_destination = &((uint8_t*) memory)[(uint32_t) destination_virtual_addr + segment_register_virtual_addr * 0x10];
        significant_destination = &((uint8_t*) memory)[(uint32_t) ((destination_virtual_addr+1)&0xFFFF) + segment_register_virtual_addr * 0x10];
    }
    else {
        significant_destination = insignificant_destination + 1;
    }

    uint8_t one = 1;
    uint8_t zero = 0;
    switch ((address_byte&0b111000) >> 3) {
        case 0b000: // INC
        {
            general_add(survivor, 1, &zero, &one, significant_destination, insignificant_destination);

            sregs.IP += ip_progress;
            return true;
        }

        case 0b001: // DEC
        {
            general_sub(survivor, 1, &zero, &one, significant_destination, insignificant_destination);

            sregs.IP += ip_progress;
            return true;
        }

        case 0b010: // CALL near
        {
            uint16_t new_IP = *insignificant_destination  + 0x100*(*significant_destination);

            sregs.IP += ip_progress;

            general_push(survivor, shared_memory, &sregs.IP);

            sregs.IP = new_IP;

            return true;
        }

        case 0b011: // CALL far
        {
            // Here we assume of course that destination_virtual_addr is initialized, that is because you can't CALL far a register

            uint16_t new_IP = memory[0].values[real_virtual_addr + 0x10000*segment]  + 0x100*memory[0].values[real_virtual_addr + 0x10000*segment + 1];
            uint16_t new_CS = memory[0].values[real_virtual_addr + 0x10000*segment + 2]  + 0x100*memory[0].values[real_virtual_addr + 0x10000*segment + 3];

            sregs.IP += ip_progress;

            general_push(survivor, shared_memory, &sregs.CS);
            general_push(survivor, shared_memory, &sregs.IP);

            sregs.CS = new_CS;
            sregs.IP = new_IP;

            return true;
        }

        case 0b100: // JMP near
        {
            sregs.IP = *insignificant_destination  + 0x100*(*significant_destination);

            return true;
        }

        case 0b101: // JMP far
        {
            // Here we assume of course that destination_virtual_addr is initialized, that is because you can't CALL far a register

            sregs.IP = memory[0].values[real_virtual_addr + 0x10000*segment] + 0x100*memory[0].values[real_virtual_addr + 0x10000*segment + 1];
            sregs.CS = memory[0].values[real_virtual_addr + 0x10000*segment + 2] + 0x100*memory[0].values[real_virtual_addr + 0x10000*segment + 3];

            return true;
        }

        case 0b110: // PUSH
        {
            uint16_t full_value = *insignificant_destination + 0x100*(*significant_destination);
            general_push(survivor, shared_memory, &full_value);

            sregs.IP += ip_progress;

            return true;
        }
    }

    return false;
}
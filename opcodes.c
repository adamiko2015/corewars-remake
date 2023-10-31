#include "structs_libraries_and_macros.h"
#include "globals.h"
#include "opcode_helper_functions.h"


bool general_binary_operation(Survivor survivor[static 1], uint16_t shared_memory) // OP *, *
{
    uint8_t opcode = memory[0].values[(sregs.IP + 10*sregs.CS) & 0xFFFF];
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
    uint8_t reg_byte = memory[0].values[(sregs.IP + 10*sregs.CS) & 0xFFFF];
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
    uint8_t reg_byte = memory[0].values[(sregs.IP + 10*sregs.CS) & 0xFFFF];
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
    debug_print_statement

    uint8_t reg_byte = memory[0].values[(sregs.IP + 10*sregs.CS) & 0xFFFF];
    uint16_t* reg = reg16_decoder(survivor, reg_byte & 0b00000111);

    return general_push(survivor, shared_memory, reg);
}

bool general_pop_opcode(Survivor survivor[static 1], uint16_t shared_memory) // POP reg16
{
    debug_print_statement

    uint8_t reg_byte = memory[0].values[(sregs.IP + 10*sregs.CS) & 0xFFFF];
    uint16_t* reg = reg16_decoder(survivor, reg_byte & 0b00000111);

    return general_pop(survivor, shared_memory, reg);
}

bool op_70(Survivor survivor[static 1], uint16_t shared_memory) // JO
{
    debug_print_statement

    bool condition = (sregs.Flags & 0x0800);
    if (condition) general_jmp_near_2B_opcode(survivor);
    else sregs.IP += 2;
    return true;
}

bool op_71(Survivor survivor[static 1], uint16_t shared_memory) // JNO
{
    debug_print_statement

    bool condition = !(sregs.Flags & 0x0800);
    if (condition) general_jmp_near_2B_opcode(survivor);
    else sregs.IP += 2;
    return true;
}

bool op_72(Survivor survivor[static 1], uint16_t shared_memory) // JC,JB,JNAE
{
    debug_print_statement

    bool condition = (sregs.Flags & 0x0001);
    if (condition) general_jmp_near_2B_opcode(survivor);
    else sregs.IP += 2;
    return true;
}

bool op_73(Survivor survivor[static 1], uint16_t shared_memory) // JNC,JNC,JAE
{
    debug_print_statement

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
    debug_print_statement

    bool condition = !(sregs.Flags & 0x0040);
    if (condition) general_jmp_near_2B_opcode(survivor);
    else sregs.IP += 2;
    return true;
}

bool op_76(Survivor survivor[static 1], uint16_t shared_memory) // JBE,JNA
{
    debug_print_statement

    bool condition = (sregs.Flags & 0x0001) || (sregs.Flags & 0x0040);
    if (condition) general_jmp_near_2B_opcode(survivor);
    else sregs.IP += 2;
    return true;
}

bool op_77(Survivor survivor[static 1], uint16_t shared_memory) // JNBE,JA
{
    debug_print_statement

    bool condition = !((sregs.Flags & 0x0001) || (sregs.Flags & 0x0040));
    if (condition) general_jmp_near_2B_opcode(survivor);
    else sregs.IP += 2;
    return true;
}

bool op_78(Survivor survivor[static 1], uint16_t shared_memory) // JS
{
    debug_print_statement

    bool condition = (sregs.Flags & 0x0080);
    if (condition) general_jmp_near_2B_opcode(survivor);
    else sregs.IP += 2;
    return true;
}

bool op_79(Survivor survivor[static 1], uint16_t shared_memory) // JNS
{
    debug_print_statement

    bool condition = !(sregs.Flags & 0x0080);
    if (condition) general_jmp_near_2B_opcode(survivor);
    else sregs.IP += 2;
    return true;
}

bool op_7A(Survivor survivor[static 1], uint16_t shared_memory) // JP,JPE
{
    debug_print_statement

    bool condition = (sregs.Flags & 0x0004);
    if (condition) general_jmp_near_2B_opcode(survivor);
    else sregs.IP += 2;
    return true;
}

bool op_7B(Survivor survivor[static 1], uint16_t shared_memory) // JNP,JPO
{
    debug_print_statement

    bool condition = !(sregs.Flags & 0x0004);
    if (condition) general_jmp_near_2B_opcode(survivor);
    else sregs.IP += 2;
    return true;
}

bool op_7C(Survivor survivor[static 1], uint16_t shared_memory) // JL,JNGE
{
    debug_print_statement

    bool condition = ((sregs.Flags & 0x0080) && 1) != ((sregs.Flags & 0x0800) && 1);
    if (condition) general_jmp_near_2B_opcode(survivor);
    else sregs.IP += 2;
    return true;
}

bool op_7D(Survivor survivor[static 1], uint16_t shared_memory) // JNL,JGE
{
    debug_print_statement

    bool condition = ((sregs.Flags & 0x0080) && 1) == ((sregs.Flags & 0x0800) && 1);
    if (condition) general_jmp_near_2B_opcode(survivor);
    else sregs.IP += 2;
    return true;
}

bool op_7E(Survivor survivor[static 1], uint16_t shared_memory) // JLE,JNG
{
    debug_print_statement

    bool condition = (sregs.Flags & 0x0040) || (((sregs.Flags & 0x0080) && 1) != ((sregs.Flags & 0x0800) && 1));
    if (condition) general_jmp_near_2B_opcode(survivor);
    else sregs.IP += 2;
    return true;
}

bool op_7F(Survivor survivor[static 1], uint16_t shared_memory) // JNLE,JG
{
    debug_print_statement

    bool condition = !(sregs.Flags & 0x0040) && ((sregs.Flags & 0x0080) && 1) == ((sregs.Flags & 0x0800) && 1);
    if (condition) general_jmp_near_2B_opcode(survivor);
    else sregs.IP += 2;
    return true;
}
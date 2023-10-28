#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdbool.h>
#include <inttypes.h>
#include <dirent.h>
#include <errno.h>

#ifndef COREWARSINTERPRETER_INIT_GLOBALS_H
#define COREWARSINTERPRETER_INIT_GLOBALS_H

#define total_team_count (teams_per_round+zombie_count)
#define exit_angrily {fprintf(stderr, "Error in file " __FILE__ " on line: %d. %s.\r\n", __LINE__, strerror(errno)); \
                        exit(1);}
#define debug_print_statement     printf("Hit file " __FILE__ " line %d\r\n", __LINE__);

#define sregs survivor->registers

#define MAX_CODE_SIZE 0x200

// might be a difference between our implementation and official implementation here.
// in the official implementation stack size and shared memory size may differ.
typedef struct {
    uint16_t AX, BX, CX, DX, SI, DI, BP, SP, CS, DS, SS, ES, IP, Energy, Flags; // Registers
    uint8_t INT86h_used, INT87h_used;
} Registers;

typedef struct  {
    bool initialized;
    uint16_t stack_id;
    Registers registers;
    char* code;
    uint16_t code_size;
} Survivor;

typedef struct {
    Survivor survivors[2];
    int8_t living_survivors[2];
    uint16_t shared_memory_id;
    uint16_t team_id; // ID of team in teams array
    char* name;
    double points;
    bool is_zombie;
} Team;

typedef struct {
    uint8_t values[0x10000];
} Segment;

struct Player_Tracker {
    int32_t team_id;
    int32_t survivor_position;
};

typedef bool (*opcode_ptr)(Survivor survivor[static 1], uint16_t shared_memory);

typedef void (*operation_ptr)(Survivor survivor[static 1], bool is_16_bit, uint8_t significant_from[static 1], uint8_t* insignificant_from,
                uint8_t significant_to[static 1], uint8_t* insignificant_to);

typedef bool (*op_generalizer)(Survivor survivor[static 1], uint16_t shared_memory, operation_ptr general_op);

#endif //COREWARSINTERPRETER_INIT_GLOBALS_H
